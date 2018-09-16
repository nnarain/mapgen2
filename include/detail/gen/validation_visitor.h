#ifndef DETAIL_GEN_VALIDATION_VISITOR_H
#define DETAIL_GEN_VALIDATION_VISITOR_H

#include "noise_gen/noise_module.h"
#include <iostream>

struct ValidationVisitor : public boost::static_visitor<bool>
{
public:
    ValidationVisitor(NoiseModule::ParameterMap& params)
        : params_{params}
    {
    }

    bool operator()(noise::module::Select& module) const
    {
        const auto lower = boost::get<float>(params_["lower_bound"]);
        const auto upper = boost::get<float>(params_["upper_bound"]);

        if (lower >= upper)
        {
            std::cout << "Invalid parameters, lower >= upper" << std::endl;
            return false;
        }

        return true;
    }

    /**
        Default
    */
    template<typename T>
    bool operator()(T&) const
    {
        return true;
    }

private:
    NoiseModule::ParameterMap& params_;
};

#endif
