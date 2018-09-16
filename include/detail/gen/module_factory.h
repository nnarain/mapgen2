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
        case NoiseModule::Type::Billow:
            return { noise::module::Billow() };
        case NoiseModule::Type::Blend:
            return { noise::module::Blend() };
        case NoiseModule::Type::Perlin:
            return { noise::module::Perlin() };
        case NoiseModule::Type::RidgedMulti:
            return { noise::module::RidgedMulti() };
        case NoiseModule::Type::ScaleBias:
            return { noise::module::ScaleBias() };
        case NoiseModule::Type::Select:
            return { noise::module::Select() };
        case NoiseModule::Type::Spheres:
            return { noise::module::Spheres() };
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
        case NoiseModule::Type::Perlin:
            return {
                {"seed", 1337},
                {"frequency", (float)noise::module::DEFAULT_PERLIN_FREQUENCY},
                {"octaves", RangedInt(1, 25, noise::module::DEFAULT_PERLIN_OCTAVE_COUNT)},
                {"persistence", RangedFloat(0.f, 1.f, noise::module::DEFAULT_PERLIN_PERSISTENCE)},
                {"lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_PERLIN_LACUNARITY)},
            };
        case NoiseModule::Type::RidgedMulti:
            return {
                { "seed", 1337 },
                { "frequency", (float)noise::module::DEFAULT_RIDGED_FREQUENCY },
                { "octaves", RangedInt(1, 25, noise::module::DEFAULT_RIDGED_OCTAVE_COUNT) },
                { "lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_RIDGED_LACUNARITY) },
            };
        case NoiseModule::Type::ScaleBias:
            return {
                {"bias", 0.0f},
                {"scale", 1.0f}
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