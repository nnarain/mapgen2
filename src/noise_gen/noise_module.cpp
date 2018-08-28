#include "noise_gen/noise_module.h"

#include <noise/module/module.h>

#include <iostream>
#include <stdexcept>

/**
    Dummy noise type used for initialization
*/
class DummyNoise : public noise::module::Module
{
public:
    DummyNoise() : Module(0)
    {

    }

    static DummyNoise instance()
    {
        static DummyNoise d;
        return d;
    }
    
    virtual int GetSourceModuleCount() const override
    {
        return 0;
    }

    virtual double GetValue(double x, double y, double z) const override
    {
        return 0.0;
    }
};

static DummyNoise dummy;

struct InvalidVistor : public boost::static_visitor<>
{
public:
    template<typename T>
    void operator()(T& operand) const
    {
        if (std::is_same<T, NoiseModule*>::value)
        {
            operand = T{};
        }
    }
};

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

    static NoiseModule::ModulePtr initModule(NoiseModule::Type type)
    {
        auto module = createModule(type);

        const auto count = module->GetSourceModuleCount();

        for (int i = 0; i < count; ++i)
        {
            module->SetSourceModule(i, dummy);
        }

        return module;
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
                {"upper_bound", 0.1f},
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

NoiseModule::NoiseModule(const std::string& name, NoiseModule::Type type) 
    : module_{ ModuleFactory::initModule(type) }
    , name_{ name }
    , type_{ type }
    , parameter_map_{ModuleFactory::initParams(type)}
    , is_valid_{false}
{
}

void NoiseModule::update()
{
    using namespace noise::module;

    is_valid_ = isValid();

    if (!is_valid_)
    {
        return;
    }
    
    Module* ptr = module_.get();
    ParameterMap& params = *parameter_map_.get();

    std::cout << name_ << " updating" << std::endl;

    switch (type_)
    {
    case NoiseModule::Type::Perlin:
        ((Perlin*)ptr)->SetSeed(boost::get<int>(params["seed"]));
        ((Perlin*)ptr)->SetFrequency(boost::get<float>(params["frequency"]));
        ((Perlin*)ptr)->SetOctaveCount(boost::get<RangedInt>(params["octaves"]).value);
        ((Perlin*)ptr)->SetPersistence(boost::get<RangedFloat>(params["persistence"]).value);
        ((Perlin*)ptr)->SetLacunarity(boost::get<RangedFloat>(params["lacunarity"]).value);
        break;
    case NoiseModule::Type::Select:
        ((Select*)ptr)->SetBounds(boost::get<float>(params["lower_bound"]), boost::get<float>(params["upper_bound"]));
        ((Select*)ptr)->SetEdgeFalloff(boost::get<float>(params["fall_off"]));
        ((Select*)ptr)->SetControlModule(*boost::get<NoiseModule*>(params["control"])->getModule().get());
        break;
    default:
        break;
    }
}

void NoiseModule::invalidateSources()
{
    std::cout << "Invalidating " << name_ << std::endl;

    int source_count = module_->GetSourceModuleCount();

    for (int i = 0; i < source_count; ++i)
    {
        module_->SetSourceModule(i, dummy);
    }

    for (auto& pair : *parameter_map_)
    {
        boost::apply_visitor(InvalidVistor{}, pair.second);
    }

    update();
}

bool NoiseModule::isValid() const
{
    ParameterMap& params = *parameter_map_.get();

    if (type_ == NoiseModule::Type::Select)
    {
        const auto lower = boost::get<float>(params["lower_bound"]);
        const auto upper = boost::get<float>(params["upper_bound"]);

        if (lower >= upper)
        {
            std::cout << "Invalid parameters, lower >= upper" << std::endl;
            return false;
        }
        
        auto* control_source = boost::get<NoiseModule*>(params["control"]);

        if (control_source == nullptr)
        {
            std::cout << "Invalid parameters, control source is null" << std::endl;
            return false;
        }

        if (control_source == this)
        {
            std::cout << "Invalid parameters, module cannot have itself as a source" << std::endl;
            return false;
        }
    }

    return true;
}

NoiseModule::ParameterMapPtr NoiseModule::getParams()
{
    return parameter_map_;
}

NoiseModule::ModulePtr& NoiseModule::getModule()
{
    return module_;
}

const std::string& NoiseModule::getName() const
{
    return name_;
}

NoiseModule::Type NoiseModule::getType() const
{
    return type_;
}