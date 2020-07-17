#include "bisect/bimo/mq/receiver.h"
#include "bisect/bimo/idioms/enforce.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
using namespace bisect::bimo;
using namespace bisect::bimo::mq;

//------------------------------------------------------------------------------

namespace
{
    constexpr auto reconnect_wait = std::chrono::seconds(2);

    AmqpClient::Channel::ptr_t init_connection(std::string_view brokerUrl, queue_info queue)
    {
        auto connection = AmqpClient::Channel::CreateFromUri(brokerUrl.data());

        BIMO_ENFORCE(connection, std::domain_error, "error connecting to broker"); // TODO: add broker
        const auto queue_id = connection->DeclareQueue(queue.name.data(), queue.passive, queue.durable, queue.exclusive,
                                                       queue.auto_delete);

        const std::string consumer_tag;
        const bool no_local                   = true;
        const bool no_ack                     = false;
        const bool exclusive                  = false;
        const uint16_t message_prefetch_count = 1;
        connection->BasicConsume(queue.name.data(), consumer_tag, no_local, no_ack, exclusive, message_prefetch_count);

        return connection;
    }
} // namespace

struct receiver::impl
{
    impl(std::string_view brokerUrl, queue_info queue, message_handler handler)
        : brokerUrl_(brokerUrl), queue_(queue), handler_(std::move(handler))
    {
        thread_ = std::thread([this]() { this->run(); });
    }

    ~impl()
    {
        stop();
        thread_.join();
    }

    void run()
    {
        while(!stop_)
        {
            try
            {
//                bllog->info("Connecting to MQ broker");
                auto connection = init_connection(brokerUrl_, queue_);
//                bllog->info("Connected to MQ broker");

                while(!stop_)
                {
                    do_consume_one(connection);
                }
            }
            catch(std::exception& ex)
            {
//                bllog->error("Error while receiving messages: {}", ex.what());
            }
            catch(...)
            {
//                bllog->error("Unknown exception while receiving messages");
            }

            if(!stop_)
            {
//                bllog->info("Retrying to connect");

                // We had a problem in the connection. Wait a bit before retrying.
                std::unique_lock<std::mutex> lock(mutex_);
//                bllog->info("Waiting to to reconnect");
                cond_.wait_for(lock, reconnect_wait, [this]() { return stop_.load(); });
            }
        }

//        bllog->info("Receiver exiting");

        std::unique_lock<std::mutex> lock(mutex_);
        done_ = true;
        cond_.notify_all();
    }

    void stop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [&]() -> bool { return done_; });
    }

    void do_consume_one(AmqpClient::Channel::ptr_t& connection)
    {
        AmqpClient::Envelope::ptr_t envelope;
        int timeout = 1000;

        const auto received = connection->BasicConsumeMessage(envelope, timeout);

        if(!received)
        {
            return;
        }

        BIMO_ENFORCE(envelope != nullptr, std::runtime_error, "couldn't consume message from queue");
//        bllog->trace("Received message: {}\n{}", envelope->RoutingKey(), envelope->Message()->Body());

        auto ack = [connection, envelope]() {
            connection->BasicAck(envelope);
//            bllog->info("Message acknowledged");
        };

        handler_(envelope->RoutingKey(), nlohmann::json::parse(envelope->Message()->Body()), ack);
    }

  private:
    const std::string brokerUrl_;
    const queue_info queue_;
    message_handler handler_;

    std::atomic<bool> stop_ = false;
    std::atomic<bool> done_ = false;
    std::mutex mutex_;
    std::condition_variable cond_;

    std::thread thread_;
};

receiver::receiver(std::string_view brokerUrl, queue_info queue, message_handler handler)
    : impl_(std::make_unique<impl>(brokerUrl, queue, std::move(handler)))
{
}

receiver::~receiver()
{
    impl_.reset();
}
