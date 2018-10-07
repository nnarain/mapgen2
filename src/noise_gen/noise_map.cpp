#include "noise_gen/noise_map.h"

#include <stdexcept>

NoiseMap::NoiseMap() :
    modules_{}
{
}

void NoiseMap::add(const std::string& name, NoiseModule::Type type)
{
    modules_[name] = NoiseModule::Ptr{ new NoiseModule(name, type) };
}

void NoiseMap::remove(const std::string& name)
{
    if (!has(name)) return;

    modules_.erase(name);

    for (auto& pair : modules_)
    {
        pair.second->update();
    }
}

void NoiseMap::rename(const std::string& current_name, const std::string& new_name)
{
    auto module = modules_[current_name];
    module->setName(new_name);

    modules_.erase(current_name);
    modules_.insert({ new_name, module });
}

void NoiseMap::setParameter(const std::string& module_name, const std::string& param_name, float value)
{
    // this assumes a float type is used
    // bad things will probably happen is the float is assigned to a non-float parameter...
    auto& params = *modules_[module_name]->getParams();
    params[param_name] = value;
}

void NoiseMap::setOutputModule(NoiseModule::Ref ref)
{
    output_ = ref;
    on_output_changed_(output_);
}

NoiseModule::Ref NoiseMap::getOutputModule() const
{
    return output_;
}

NoiseModule::Ptr& NoiseMap::createModuleWithUniqueName(NoiseModule::Type type)
{
    std::size_t i = 0;
    std::string name;

    do
    {
        name = std::string("noise") + std::to_string(i);
        i++;
    }
    while (has(name));

    add(name, type);

    return get(name);
}

NoiseModule::Ptr& NoiseMap::get(const std::string& name)
{
    if (modules_.find(name) != modules_.end())
    {
        return modules_[name];
    }
    else
    {
        throw std::runtime_error("No noise module with given name");
    }
}

bool NoiseMap::has(const std::string& name) const
{
    return modules_.find(name) != modules_.end();
}

void NoiseMap::forEach(std::function<void(const std::string&, NoiseModule&)> fn)
{
    for (auto& it : modules_)
    {
        fn(it.first, *it.second);
    }
}

void NoiseMap::setSeed(int seed) noexcept
{
    for (auto& pair : modules_)
    {
        auto& module = pair.second;
        module->setSeed(seed++);
    }
}

void NoiseMap::connectOutputChanged(std::function<void(NoiseModule::Ref)> fn)
{
    on_output_changed_.connect(fn);
}

std::size_t NoiseMap::size() const noexcept
{
    return modules_.size();
}