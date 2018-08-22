#include "noise_gen/noise_module.h"

#include <noise/module/module.h>

#include <stdexcept>

/**
    Create libnoise modules and allocation parameters
*/
class ModuleFactory
{
public:
    static NoiseModule::ModulePtr createModule(NoiseModule::Type type)
    {
        switch (type)
        {
        case NoiseModule::Type::Perlin:
            return std::make_unique<noise::module::Perlin>();
        case NoiseModule::Type::Select:
            return std::make_unique<noise::module::Select>();
        default:
            throw std::runtime_error("Invalid noise type");
            break;
        }
    }

    static NoiseModule::ParameterMap createParams(NoiseModule::Type type)
    {
        switch (type)
        {
        case NoiseModule::Type::Perlin:
            return {
                {"seed", 1337},
                {"frequency", 0.1f},
                {"octaves", RangedInt(1, 25, 1)},
                {"persistence", RangedFloat(0.f, 1.f, 0.5f)},
                {"lacunarity", RangedFloat(1.f, 2.f, 2.f)},
            };
        case NoiseModule::Type::Select:
            return {
                {"lower_bound", 0.0f},
                {"upper_bound", 0.0f},
                {"control", nullptr},
                {"fall_off", 0.0f}
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

NoiseModule::NoiseModule(const std::string& name, NoiseModule::Type type) :
    module_{ ModuleFactory::createModule(type) },
    name_{ name },
    type_{ type },
    parameter_map_{ModuleFactory::initParams(type)}
{
}

void NoiseModule::update()
{
    using namespace noise::module;
    
    Module* ptr = module_.get();
    ParameterMap& params = *parameter_map_.get();

    switch (type_)
    {
    case NoiseModule::Type::Perlin:
        ((Perlin*)ptr)->SetSeed(boost::get<int>(params["seed"]));
        ((Perlin*)ptr)->SetFrequency(boost::get<float>(params["frequency"]));
        ((Perlin*)ptr)->SetOctaveCount(boost::get<int>(params["octaves"]));
        ((Perlin*)ptr)->SetPersistence(boost::get<float>(params["persistence"]));
        ((Perlin*)ptr)->SetLacunarity(boost::get<float>(params["lacunarity"]));
        break;
    case NoiseModule::Type::Select:
        ((Select*)ptr)->SetBounds(boost::get<float>(params["lower_bound"]), boost::get<float>(params["upper_bound"]));
        ((Select*)ptr)->SetEdgeFalloff(boost::get<float>(params["fall_off"]));
        ((Select*)ptr)->SetControlModule(*boost::get<Module*>(params["control"]));
        break;
    default:
        break;
    }
}

NoiseModule::ParameterMapPtr NoiseModule::getParams()
{
    return parameter_map_;
}