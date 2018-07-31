#include "bisect/bimo/memory/telemetry.h"
using namespace bisect::bimo;

//------------------------------------------------------------------------------

telemetry& telemetry::instance()
{
    static telemetry instance;
    return instance;
}

telemetry::~telemetry()
{
}

void telemetry::on_direct_allocation(size_t /*_size*/)
{
}

void telemetry::on_allocation_through_factory(size_t /*_size*/)
{
}

