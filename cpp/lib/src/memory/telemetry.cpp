#include "bisect/bimo/memory/telemetry.h"
using namespace bisect::bimo;

//#define EBU_LIST_ENABLE_TELEMETRY

//------------------------------------------------------------------------------
void allocation_telemetry::dump()
{
    printf("Allocations: %d\n", counter_.load());

    for (const auto[size, count] : histogram_)
    {
        printf("Size: %lld\tCount: %d\n", size, count);
    }
}

void allocation_telemetry::on_allocation(size_t _size)
{
    ++counter_;

    const auto it = histogram_.find(_size);
    if (it == histogram_.end())
    {
        histogram_.insert({ _size, 1 });
    }
    else
    {
        const auto v = it->second + 1;
        histogram_.insert_or_assign(_size, v);
    }
}
//------------------------------------------------------------------------------

telemetry& telemetry::instance()
{
    static telemetry instance;
    return instance;
}

#if defined(EBU_LIST_ENABLE_TELEMETRY)

telemetry::~telemetry()
{
    printf("Direct allocation:\n");
    direct_allocation_.dump();
    printf("Allocation through factory:\n");
    allocation_through_factory_.dump();
}

void telemetry::on_direct_allocation(size_t _size)
{
    direct_allocation_.on_allocation(_size);
}

void telemetry::on_allocation_through_factory(size_t _size)
{
    allocation_through_factory_.on_allocation(_size);
}

#else // defined(EBU_LIST_ENABLE_TELEMETRY)

telemetry::~telemetry()
{
}

void telemetry::on_direct_allocation(size_t /*_size*/)
{
}

void telemetry::on_allocation_through_factory(size_t /*_size*/)
{
}

#endif // defined(EBU_LIST_ENABLE_TELEMETRY)

