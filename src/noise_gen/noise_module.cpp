#include "noise_gen/noise_module.h"
#include "detail/gen/source_counter_visitor.h"
#include "detail/gen/validation_visitor.h"
#include "detail/gen/set_param_visitor.h"

#include <noise/module/module.h>

#include <algorithm>
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

    static DummyNoise& instance()
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

struct ModuleRefVistor : public boost::static_visitor<noise::module::Module&>
{
public:
    template<typename T>
    noise::module::Module& operator()(T& module) const
    {
        return static_cast<noise::module::Module&>(module);
    }
};

struct InvalidateVistor : public boost::static_visitor<>
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
    static NoiseModule::ModuleVariant createModule(NoiseModule::Type type)
    {
        switch (type)
        {
        case NoiseModule::Type::Billow:
            return { noise::module::Billow() };
        case NoiseModule::Type::Perlin:
            return { noise::module::Perlin() };
        case NoiseModule::Type::RidgedMulti:
            return { noise::module::RidgedMulti() };
        case NoiseModule::Type::ScaleBias:
            return { noise::module::ScaleBias() };
        case NoiseModule::Type::Select:
            return { noise::module::Select() };
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
                { "frequency", 0.1f },
                { "octaves", RangedInt(1, 25, 1) },
                { "persistence", RangedFloat(0.f, 1.f, 0.5f) },
                { "lacunarity", RangedFloat(1.f, 2.f, 2.f) },
            };
        case NoiseModule::Type::Perlin:
            return {
                {"seed", 1337},
                {"frequency", 0.1f},
                {"octaves", RangedInt(1, 25, 1)},
                {"persistence", RangedFloat(0.f, 1.f, 0.5f)},
                {"lacunarity", RangedFloat(1.f, 2.f, 2.f)},
            };
        case NoiseModule::Type::RidgedMulti:
            return {
                { "seed", 1337 },
                { "frequency", 0.1f },
                { "octaves", RangedInt(1, 25, 1) },
                { "lacunarity", RangedFloat(1.f, 2.f, 2.f) },
            };
        case NoiseModule::Type::ScaleBias:
            return {
                {"bias", 0.0f},
                {"scale", 1.0f}
            };
        case NoiseModule::Type::Select:
            return {
                {"lower_bound", 0.0f},
                {"upper_bound", 0.1f},
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
    : module_base_{ ModuleFactory::createModule(type) }
    , module_{ boost::apply_visitor(ModuleRefVistor{}, module_base_) }
    , source_refs_(module_.GetSourceModuleCount())
    , name_{ name }
    , type_{ type }
    , parameter_map_{ModuleFactory::initParams(type)}
    , is_valid_{false}
    , actual_source_count_{0}
{
    actual_source_count_ = module_.GetSourceModuleCount();

    const auto count = module_.GetSourceModuleCount();

    for (int i = 0; i < count; ++i)
    {
        module_.SetSourceModule(i, dummy);
    }
}

void NoiseModule::update()
{
    using namespace noise::module;

    is_valid_ = validate();

    if (!is_valid_)
    {
        return;
    }
    
    std::cout << name_ << " updating" << std::endl;
    boost::apply_visitor(SetParamsVisitor{ *parameter_map_ }, module_base_);
}

NoiseModule::ParameterMapPtr NoiseModule::getParams()
{
    return parameter_map_;
}

noise::module::Module& NoiseModule::getModule()
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

bool NoiseModule::validate()
{
    auto valid_params = boost::apply_visitor(ValidationVisitor{ *parameter_map_ }, module_base_);
    auto valid_sources = std::all_of(source_refs_.begin(), source_refs_.begin() + actual_source_count_,
        [](NoiseModule::Ref ref) 
        {
            return !ref.expired();
        }
    );

    return valid_params && valid_sources;
}

bool NoiseModule::isValid() const
{
    return is_valid_;
}

void NoiseModule::setSourceModule(int index, NoiseModule::Ptr& ptr)
{
    source_refs_[index] = NoiseModule::Ref{ ptr };
    module_.SetSourceModule(index, ptr->getModule());
}

int NoiseModule::getSourceModuleCount()
{
    return actual_source_count_;
}

NoiseModule::Ref NoiseModule::getSourceModule(int index)
{
    return source_refs_[index];
}

int NoiseModule::getActualSourceCount()
{
    SourceParamCounterVistor source_param_counter;
    for (auto& pair : *parameter_map_)
    {
        boost::apply_visitor(source_param_counter, pair.second);
    }

    auto count = module_.GetSourceModuleCount() - source_param_counter.getCount();

    return count;
}