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

    bool operator()(noise::module::Perlin& module) const { return true; }

    bool operator()(noise::module::Select& module) const
    {
        const auto lower = boost::get<float>(params_["lower_bound"]);
        const auto upper = boost::get<float>(params_["upper_bound"]);

        if (lower >= upper)
        {
            std::cout << "Invalid parameters, lower >= upper" << std::endl;
            return false;
        }

        auto control_source = boost::get<NoiseModule::Ref>(params_["control"]);

        if (auto ptr = control_source.lock())
        {
            if (&ptr->getModule() == &module)
            {
                std::cout << "Invalid parameters, module cannot have itself as a source" << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "Invalid parameters, control source is null" << std::endl;
            return false;
        }

        return true;
    }

private:
    NoiseModule::ParameterMap& params_;
};

#endif
