#include "noise_gen/noise_module.h"
#include "detail/gen/source_counter_visitor.h"
#include "detail/gen/validation_visitor.h"
#include "detail/gen/set_param_visitor.h"
#include "detail/gen/dummy_noise.h"
#include "detail/gen/module_ref_visitor.h"
#include "detail/gen/module_factory.h"
#include "detail/gen/seed_setter_visitor.h"


#include <noise/module/module.h>

#include <algorithm>
#include <stdexcept>


// Used for initialization of modules
static DummyNoise dummy;


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
    
    boost::apply_visitor(SetParamsVisitor{ *parameter_map_ }, module_base_);
}

bool NoiseModule::updateParameters(std::function<bool(const std::string&, ParameterVariant&)> fn)
{
    bool update_required = false;

    // pass individual parameters to client function
    for (auto& param : *parameter_map_)
    {
        auto updated = fn(param.first, param.second);
        update_required = update_required || updated;
    }

    if (update_required)
    {
        update();
    }

    return update_required;
}

void NoiseModule::setSeed(int seed)
{
    boost::apply_visitor(detail::gen::SeedSetterVisitor{seed}, module_base_);
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

void NoiseModule::setName(const std::string& name)
{
    this->name_ = name;
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

void NoiseModule::setSourceModule(int index, NoiseModule::Ptr ptr)
{
    source_refs_[index] = NoiseModule::Ref{ ptr };
    if (ptr)
    {
        module_.SetSourceModule(index, ptr->getModule());
    }

    is_valid_ = validate();
}

int NoiseModule::getSourceModuleCount()
{
    return actual_source_count_;
}

NoiseModule::Ref NoiseModule::getSourceModule(int index)
{
    return source_refs_[index];
}
