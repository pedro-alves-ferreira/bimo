#pragma once

#include <string>

namespace bisect::bimo::mq
{
    struct queue_info
    {
        std::string_view name;
        bool passive     = false;
        bool durable     = false;
        bool exclusive   = false;
        bool auto_delete = false;
    };

    struct exchange_info
    {
        std::string_view name;
        std::string_view type;
        bool passive     = false;
        bool durable     = false;
        bool auto_delete = false;
    };
} // namespace bisect::bimo::mq
