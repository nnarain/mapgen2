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

    bool operator()(noise::module::Billow& module) const { return true; }
    bool operator()(noise::module::Blend& module) const { return true; }
    bool operator()(noise::module::Perlin& module) const { return true; }
    bool operator()(noise::module::RidgedMulti& module) const { return true; }
    bool operator()(noise::module::ScaleBias& module) const { return true; }

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

    bool operator()(noise::module::Spheres& module) const { return true; }
    bool operator()(noise::module::Turbulence& module) const { return true; }
    bool operator()(noise::module::Voronoi& module) const { return true; }

private:
    NoiseModule::ParameterMap& params_;
};

#endif
