#pragma once

#include "bisect/bimo/mq/common.h"
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>

namespace bisect::bimo::mq
{
    // Ack callback to be called by the handler if the message is to be
    // acknowledge. it will not. If not called before the function returns, the
    // message will not be acknowledged.
    using ack_callback = std::function<void()>;
    using message_handler =
        std::function<void(const std::string& routing_key, const nlohmann::json& message, const ack_callback&)>;

    class receiver
    {
      public:
        receiver(std::string_view brokerUrl, queue_info queue, message_handler handler);
        ~receiver();

      private:
        struct impl;
        std::unique_ptr<impl> impl_;
    };
} // namespace bisect::bimo::mq
