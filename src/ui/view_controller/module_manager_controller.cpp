#include "ui/view_controller/module_manager_controller.h"


ModuleManagerController::ModuleManagerController(ModuleManager& manager)
    : manager_{manager}
    , names_{}
    , is_names_cached_{false}
{

}

NoiseModule::Ptr& ModuleManagerController::get(const std::string& name)
{
    return manager_.get(name);
}

bool ModuleManagerController::has(const std::string& name)
{
    return manager_.has(name);
}

void ModuleManagerController::createModule(const std::string& name, NoiseModule::Type type)
{
    manager_.create(name, type);
    is_names_cached_ = false;
}

void ModuleManagerController::removeModule(const std::string& name)
{
    manager_.remove(name);
    is_names_cached_ = false;
}

const std::vector<std::string>& ModuleManagerController::getModuleNames()
{
    if (is_names_cached_)
    {
        // names are cached, return cache names
        return names_;
    }
    else
    {
        names_.clear();

        manager_.forEach([this](const std::string& name, NoiseModule&)
        {
            names_.push_back(name);
        });

        is_names_cached_ = true;

        return names_;
    }
}