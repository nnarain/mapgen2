#ifndef DETAIL_GEN_SOURCE_COUNTER_VISITOR_H
#define DETAIL_GEN_SOURCE_COUNTER_VISITOR_H

#include "noise_gen/noise_module.h"
#include "noise_gen/parameter.h"
#include <boost/variant.hpp>

struct SourceParamCounterVistor : public boost::static_visitor<>
{
public:
    SourceParamCounterVistor() : count_{ 0 } {}
    void operator()(int&){}
    void operator()(float&){}
    void operator()(RangedInt&){}
    void operator()(RangedFloat&){}

    void operator()(NoiseModule::Ref&)
    {
        count_++;
    }

    int getCount() const
    {
        return count_;
    }

private:
    int count_;
};

#endif