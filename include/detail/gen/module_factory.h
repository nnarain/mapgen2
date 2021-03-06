#ifndef DETAIL_GEN_MODULE_FACTORY_H
#define DETAIL_GEN_MODULE_FACTORY_H

#include "noise_gen/noise_module.h"

/**
    Create libnoise modules and allocation parameters
*/
class ModuleFactory
{
public:
    static NoiseModule::ModuleVariant createModule(NoiseModule::Type type)
    {
        switch (type)
        {
        case NoiseModule::Type::Abs:
            return { noise::module::Abs() };
        case NoiseModule::Type::Add:
            return { noise::module::Add() };
        case NoiseModule::Type::Billow:
            return { noise::module::Billow() };
        case NoiseModule::Type::Blend:
            return { noise::module::Blend() };
        case NoiseModule::Type::Cache:
            return { noise::module::Cache() };
        case NoiseModule::Type::Checkerboard:
            return { noise::module::Checkerboard() };
        case NoiseModule::Type::Clamp:
            return { noise::module::Clamp() };
        case NoiseModule::Type::Const:
            return { noise::module::Const() };
        case NoiseModule::Type::Cylinders:
            return { noise::module::Cylinders() };
        case NoiseModule::Type::Displace:
            return { noise::module::Displace() };
        case NoiseModule::Type::Exponent:
            return { noise::module::Exponent() };
        case NoiseModule::Type::Invert:
            return { noise::module::Invert() };
        case NoiseModule::Type::Max:
            return { noise::module::Max() };
        case NoiseModule::Type::Min:
            return { noise::module::Min() };
        case NoiseModule::Type::Multiply:
            return { noise::module::Multiply() };
        case NoiseModule::Type::Perlin:
            return { noise::module::Perlin() };
        case NoiseModule::Type::Power:
            return { noise::module::Power() };
        case NoiseModule::Type::RidgedMulti:
            return { noise::module::RidgedMulti() };
        case NoiseModule::Type::RotatePoint:
            return { noise::module::RotatePoint() };
        case NoiseModule::Type::ScaleBias:
            return { noise::module::ScaleBias() };
        case NoiseModule::Type::ScalePoint:
            return { noise::module::ScalePoint() };
        case NoiseModule::Type::Select:
            return { noise::module::Select() };
        case NoiseModule::Type::Spheres:
            return { noise::module::Spheres() };
        case NoiseModule::Type::TranslatePoint:
            return { noise::module::TranslatePoint() };
        case NoiseModule::Type::Turbulence:
            return { noise::module::Turbulence() };
        case NoiseModule::Type::Voronoi:
            return { noise::module::Voronoi() };
        default:
            throw std::runtime_error("Invalid noise type");
            break;
        }
    }

    static NoiseModule::ParameterMap createParams(NoiseModule::Type type)
    {
        switch (type)
        {
        case NoiseModule::Type::Abs:
            return {
            };
        case NoiseModule::Type::Add:
            return {
            };
        case NoiseModule::Type::Billow:
            return {
                { "seed", 1337 },
                { "frequency", (float)noise::module::DEFAULT_BILLOW_FREQUENCY },
                { "octaves", RangedInt(1, 25, noise::module::DEFAULT_BILLOW_OCTAVE_COUNT) },
                { "persistence", RangedFloat(0.f, 1.f, noise::module::DEFAULT_BILLOW_PERSISTENCE) },
                { "lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_BILLOW_LACUNARITY) },
            };
        case NoiseModule::Type::Blend:
            return {
            };
        case NoiseModule::Type::Cache:
            return {
            };
        case NoiseModule::Type::Checkerboard:
            return {
            };
        case NoiseModule::Type::Clamp:
            return {
                {"lower_bound", 0.0f},
                {"upper_bound", 0.0f}
            };
        case NoiseModule::Type::Const:
            return {
                {"value", (float)noise::module::DEFAULT_CONST_VALUE}
            };
        case NoiseModule::Type::Cylinders:
            return {
                {"frequency", (float)noise::module::DEFAULT_CYLINDERS_FREQUENCY}
            };
        case NoiseModule::Type::Displace:
            return {
            };
        case NoiseModule::Type::Exponent:
            return {
                {"exp", (float)noise::module::DEFAULT_EXPONENT}
            };
        case NoiseModule::Type::Invert:
            return {
            };
        case NoiseModule::Type::Max:
            return {
            };
        case NoiseModule::Type::Min:
            return {
            };
        case NoiseModule::Type::Multiply:
            return {
            };
        case NoiseModule::Type::Perlin:
            return {
                {"seed", 1337},
                {"frequency", (float)noise::module::DEFAULT_PERLIN_FREQUENCY},
                {"octaves", RangedInt(1, 25, noise::module::DEFAULT_PERLIN_OCTAVE_COUNT)},
                {"persistence", RangedFloat(0.f, 1.f, noise::module::DEFAULT_PERLIN_PERSISTENCE)},
                {"lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_PERLIN_LACUNARITY)},
            };
        case NoiseModule::Type::Power:
            return {
            };
        case NoiseModule::Type::RidgedMulti:
            return {
                { "seed", 1337 },
                { "frequency", (float)noise::module::DEFAULT_RIDGED_FREQUENCY },
                { "octaves", RangedInt(1, 25, noise::module::DEFAULT_RIDGED_OCTAVE_COUNT) },
                { "lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_RIDGED_LACUNARITY) },
            };
        case NoiseModule::Type::RotatePoint:
            return {
                { "x", (float)noise::module::DEFAULT_ROTATE_X },
                { "y", (float)noise::module::DEFAULT_ROTATE_Y },
                { "z", (float)noise::module::DEFAULT_ROTATE_Z },
            };
        case NoiseModule::Type::ScaleBias:
            return {
                {"bias", 0.0f},
                {"scale", 1.0f}
            };
        case NoiseModule::Type::ScalePoint:
            return {
                { "x", (float)noise::module::DEFAULT_SCALE_POINT_X },
                { "y", (float)noise::module::DEFAULT_SCALE_POINT_Y },
                { "z", (float)noise::module::DEFAULT_SCALE_POINT_Z },
            };
        case NoiseModule::Type::Select:
            return {
                {"lower_bound", (float)noise::module::DEFAULT_SELECT_LOWER_BOUND},
                {"upper_bound", (float)noise::module::DEFAULT_SELECT_UPPER_BOUND},
                {"fall_off", (float)noise::module::DEFAULT_SELECT_EDGE_FALLOFF}
            };
        case NoiseModule::Type::Spheres:
            return {
                {"frequency", (float)noise::module::DEFAULT_SPHERES_FREQUENCY}
            };
        case NoiseModule::Type::TranslatePoint:
            return {
                { "x", (float)noise::module::DEFAULT_TRANSLATE_POINT_X },
                { "y", (float)noise::module::DEFAULT_TRANSLATE_POINT_X },
                { "z", (float)noise::module::DEFAULT_TRANSLATE_POINT_X },
            };
        case NoiseModule::Type::Turbulence:
            return {
                {"seed", 1337},
                {"frequency", (float)noise::module::DEFAULT_TURBULENCE_FREQUENCY},
                {"power", (float)noise::module::DEFAULT_TURBULENCE_POWER},
                {"roughness", (float)noise::module::DEFAULT_TURBULENCE_ROUGHNESS}
            };
        case NoiseModule::Type::Voronoi:
            return {
                {"seed", 1337},
                {"displacement", (float)noise::module::DEFAULT_VORONOI_DISPLACEMENT},
                {"frequency", (float)noise::module::DEFAULT_VORONOI_FREQUENCY},
                {"enable_distance", false}
            };
        default:
            throw std::runtime_error("Invalid noise type");
            break;
        }
    }

    static NoiseModule::ParameterMapPtr initParams(NoiseModule::Type type)
    {
        auto params = createParams(type);
        return std::make_shared<NoiseModule::ParameterMap>(params);
    }
};

#endif