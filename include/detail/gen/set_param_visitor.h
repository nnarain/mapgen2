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

    void operator()(noise::module::Billow& module) const
    {
        module.SetSeed(get<int>("seed"));
        module.SetFrequency(get<float>("frequency"));
        module.SetOctaveCount(get<RangedInt>("octaves").value);
        module.SetPersistence(get<RangedFloat>("persistence").value);
        module.SetLacunarity(get<RangedFloat>("lacunarity").value);
    }

    void operator()(noise::module::Clamp& module) const
    {
        module.SetBounds(get<float>("lower_bound"), get<float>("upper_bound"));
    }

    void operator()(noise::module::Const& module) const
    {
        module.SetConstValue(get<float>("value"));
    }

    void operator()(noise::module::Cylinders& module) const
    {
        module.SetFrequency(get<float>("frequency"));
    }

    void operator()(noise::module::Exponent& module) const
    {
        module.SetExponent(get<float>("exp"));
    }

    void operator()(noise::module::Perlin& module) const
    {
        module.SetSeed(get<int>("seed"));
        module.SetFrequency(get<float>("frequency"));
        module.SetOctaveCount(get<RangedInt>("octaves").value);
        module.SetPersistence(get<RangedFloat>("persistence").value);
        module.SetLacunarity(get<RangedFloat>("lacunarity").value);
    }

    void operator()(noise::module::RidgedMulti& module) const
    {
        module.SetSeed(get<int>("seed"));
        module.SetFrequency(get<float>("frequency"));
        module.SetOctaveCount(get<RangedInt>("octaves").value);
        module.SetLacunarity(get<RangedFloat>("lacunarity").value);
    }

    void operator()(noise::module::ScaleBias& module) const
    {
        module.SetBias(get<float>("bias"));
        module.SetScale(get<float>("scale"));
    }

    void operator()(noise::module::Select& module) const
    {
        module.SetBounds(get<float>("lower_bound"), get<float>("upper_bound"));
        module.SetEdgeFalloff(get<float>("fall_off"));
    }

    void operator()(noise::module::Spheres& module) const
    {
        module.SetFrequency(get<float>("frequency"));
    }

    void operator()(noise::module::RotatePoint& module) const
    {
        module.SetXAngle(get<float>("x"));
        module.SetYAngle(get<float>("y"));
        module.SetZAngle(get<float>("z"));
    }

    void operator()(noise::module::ScalePoint& module) const
    {
        module.SetXScale(get<float>("x"));
        module.SetYScale(get<float>("y"));
        module.SetZScale(get<float>("z"));
    }


    void operator()(noise::module::Turbulence& module) const
    {
        module.SetSeed(get<int>("seed"));
        module.SetFrequency(get<float>("frequency"));
        module.SetPower(get<float>("power"));
        module.SetRoughness(get<float>("roughness"));
    }

    void operator()(noise::module::Voronoi& module) const
    {
        module.SetSeed(get<int>("seed"));
        module.SetFrequency(get<float>("frequency"));
        module.SetDisplacement(get<float>("displacement"));
        module.EnableDistance(get<bool>("enable_distance"));
    }

    /**
        Default
    */
    template<typename T>
    void operator()(T&) const
    {
    }

private:
    template<typename T>
    T get(const std::string& name) const
    {
        return boost::get<T>(params_[name]);
    }

    NoiseModule::ParameterMap& params_;
};

#endif
