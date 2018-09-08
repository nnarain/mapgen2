#ifndef DETAIL_GEN_PARAM_VISITOR_H
#define DETAIL_GEN_PARAM_VISITOR_H

#include "noise_gen/noise_module.h"

struct SetParamsVisitor : public boost::static_visitor<>
{
public:
    SetParamsVisitor(NoiseModule::ParameterMap& params)
        : params_{params}
    {

    }

    void operator()(noise::module::Perlin& module) const
    {
        module.SetSeed(boost::get<int>(params_["seed"]));
        module.SetFrequency(boost::get<float>(params_["frequency"]));
        module.SetOctaveCount(boost::get<RangedInt>(params_["octaves"]).value);
        module.SetPersistence(boost::get<RangedFloat>(params_["persistence"]).value);
        module.SetLacunarity(boost::get<RangedFloat>(params_["lacunarity"]).value);
    }

    void operator()(noise::module::Select& module) const
    {
        module.SetBounds(boost::get<float>(params_["lower_bound"]), boost::get<float>(params_["upper_bound"]));
        module.SetEdgeFalloff(boost::get<float>(params_["fall_off"]));
    //    module.SetControlModule(boost::get<NoiseModule::Ref>(params_["control"]).lock()->getModule());
    }
private:
    NoiseModule::ParameterMap& params_;
};

#endif
