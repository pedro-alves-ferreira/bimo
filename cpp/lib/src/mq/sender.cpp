#include "bisect/bimo/mq/sender.h"
#include "bisect/bimo/mq/common.h"
#include "bisect/bimo/idioms/enforce.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using namespace bisect::bimo::mq;
using namespace bisect::bimo;

//------------------------------------------------------------------------------

namespace
{
    AmqpClient::Channel::ptr_t init_channel(std::string_view brokerUrl, queue_info queue)
    {
        auto channel = AmqpClient::Channel::CreateFromUri(brokerUrl.data());

        BIMO_ENFORCE(channel, std::domain_error, "error connecting to {}"); // TODO: add brokerUrl
        const auto queue_id =
            channel->DeclareQueue(queue.name.data(), queue.passive, queue.durable, queue.exclusive, queue.auto_delete);
//        bllog->trace("Queue {}", queue_id);

        return channel;
    }

    AmqpClient::Channel::ptr_t init_channel(std::string_view brokerUrl, exchange_info exchange)
    {
        auto channel = AmqpClient::Channel::CreateFromUri(brokerUrl.data());

        BIMO_ENFORCE(channel, std::domain_error, "error connecting to {}"); // TODO add brokerUrl

        channel->DeclareExchange(exchange.name.data(), exchange.type.data(), exchange.passive, exchange.durable,
                                 exchange.auto_delete);

        return channel;
    }
} // namespace

//------------------------------------------------------------------------------

struct sender::impl
{
    impl(std::string_view brokerUrl, queue_info queue)
        : brokerUrl_(brokerUrl), queue_(queue), channel_(init_channel(brokerUrl, queue))
    {
    }

    void send(std::string_view message)
    {
        auto m = AmqpClient::BasicMessage::Create(message.data());
        channel_->BasicPublish("", queue_.name.data(), m, false, false);
    }

  private:
    const std::string brokerUrl_;
    const queue_info queue_;
    AmqpClient::Channel::ptr_t channel_;
};

sender::sender(std::string_view brokerUrl, queue_info queue) : impl_(std::make_unique<impl>(brokerUrl, queue))
{
}

sender::~sender()
{
    impl_.reset();
}

void sender::send(std::string_view message)
{
    impl_->send(message);
}

//------------------------------------------------------------------------------

struct exchange_sender::impl
{
    impl(std::string_view brokerUrl, exchange_info exchange) : brokerUrl_(brokerUrl), exchange_(exchange) {}

    void send(std::string_view key, std::string_view message)
    {
        try
        {
            if(!channel_)
            {
                channel_ = init_channel(brokerUrl_, exchange_);
            }

            auto m = AmqpClient::BasicMessage::Create(message.data());
            channel_->BasicPublish(exchange_.name.data(), key.data(), m);
        }
        catch(std::exception& ex)
        {
//            bllog->error("Error while sending message: {}", ex.what());
            channel_ = nullptr;
        }
        catch(...)
        {
//            bllog->error("Unknown exception while sending message");
            channel_ = nullptr;
        }
    }

  private:
    const std::string brokerUrl_;
    const exchange_info exchange_;
    AmqpClient::Channel::ptr_t channel_;
};

exchange_sender::exchange_sender(std::string_view brokerUrl, exchange_info exchange)
    : impl_(std::make_unique<impl>(brokerUrl, exchange))
{
}

exchange_sender::~exchange_sender()
{
    impl_.reset();
}

void exchange_sender::send(std::string_view key, std::string_view message)
{
    impl_->send(key, message);
}
