#include "noise_gen/noise_module.h"
#include "detail/gen/source_counter_visitor.h"

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

struct ModuleRefVistor : public boost::static_visitor<noise::module::Module&>
{
public:
    template<typename T>
    noise::module::Module& operator()(T& module) const
    {
        return static_cast<noise::module::Module&>(module);
    }
};

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

struct SetParamsVistor : public boost::static_visitor<>
{
public:
    SetParamsVistor(NoiseModule::ParameterMap& params)
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
        module.SetControlModule(boost::get<NoiseModule::Ref>(params_["control"]).lock()->getModule());
    }
private:
    NoiseModule::ParameterMap& params_;
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
        case NoiseModule::Type::Perlin:
            return { noise::module::Perlin() };
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
                {"control", NoiseModule::Ref{}},
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
    actual_source_count_ = getActualSourceCount();

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
    boost::apply_visitor(SetParamsVistor{ *parameter_map_ }, module_base_);
}

void NoiseModule::invalidateSources()
{
    std::cout << "Invalidating " << name_ << std::endl;

    int source_count = module_.GetSourceModuleCount();

    for (int i = 0; i < source_count; ++i)
    {
        module_.SetSourceModule(i, dummy);
    }

    for (auto& pair : *parameter_map_)
    {
        boost::apply_visitor(InvalidateVistor{}, pair.second);
    }

    update();
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