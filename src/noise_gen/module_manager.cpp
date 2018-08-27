#include "noise_gen/module_manager.h"

#include <stdexcept>

ModuleManager::ModuleManager() :
    modules_{}
{
}

void ModuleManager::create(const std::string& name, NoiseModule::Type type)
{
    modules_[name] = std::make_unique<NoiseModule>(name, type);
}

void ModuleManager::remove(const std::string& name)
{
    // before removing the noise module, it must be removed as a parameter from the other noise modules
    auto module_to_remove = modules_[name]->getModule().get();
    
    for (auto& pair : modules_)
    {
        auto& module = pair.second;
        auto& base = module->getModule();
        
        int source_count = base->GetSourceModuleCount();

        // determine if the source is the module to remove
        for (int i = 0; i < source_count; ++i)
        {
            auto source = &base->GetSourceModule(i);

            if (source == module_to_remove)
            {
                module->invalidateSources();
            }
        }
    }

    modules_.erase(name);
}

NoiseModule::Ptr& ModuleManager::get(const std::string& name)
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

bool ModuleManager::has(const std::string& name) const
{
    return modules_.find(name) != modules_.end();
}

void ModuleManager::forEach(std::function<void(const std::string&, NoiseModule&)> fn)
{
    for (auto& it : modules_)
    {
        fn(it.first, *it.second.get());
    }
}