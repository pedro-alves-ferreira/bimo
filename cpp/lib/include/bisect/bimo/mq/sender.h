#pragma once

#include "bisect/bimo/mq/common.h"
#include <memory>
#include <nlohmann/json.hpp>

namespace bisect::bimo::mq
{
    class sender
    {
      public:
        sender(std::string_view brokerUrl, queue_info queue);
        ~sender();

        void send(std::string_view message);

      private:
        struct impl;
        std::unique_ptr<impl> impl_;
    };

    class exchange_sender
    {
      public:
        exchange_sender(std::string_view brokerUrl, exchange_info exchange);
        ~exchange_sender();

        void send(std::string_view key, std::string_view message);

      private:
        struct impl;
        std::unique_ptr<impl> impl_;
    };
} // namespace bisect::bimo::mq
