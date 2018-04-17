#pragma once

#include <atomic>
#include <map>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class allocation_telemetry
    {
    public:
        void dump();
        void on_allocation(size_t _size);

    private:
        std::atomic<int> counter_ = 0;
        std::map<size_t, int> histogram_;
    };

    class telemetry
    {
    public:
        ~telemetry();
        static telemetry& instance();
        
        void on_direct_allocation(size_t _size);
        void on_allocation_through_factory(size_t _size);

    private:
        telemetry() = default;
        allocation_telemetry direct_allocation_;
        allocation_telemetry allocation_through_factory_;
    };
}
