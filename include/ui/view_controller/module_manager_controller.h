#ifndef UI_VIEW_CONTROLLER_MODULE_MANAGER_VIEW_CONTROLLER_H
#define UI_VIEW_CONTROLLER_MODULE_MANAGER_VIEW_CONTROLLER_H

#include "noise_gen/module_manager.h"

#include <string>
#include <vector>
#include <map>

/**
    Interface to ModuleManager
*/
class ModuleManagerController
{
public:
    ModuleManagerController(ModuleManager& manager);

    NoiseModule& get(const std::string& name);
    bool has(const std::string& name);

    void createModule(const std::string& name, NoiseModule::Type type);
    void removeModule(const std::string& name);

    const std::vector<std::string>& getModuleNames();
    const std::string& lookupName(const noise::module::Module&);

private:
    ModuleManager& manager_;

    std::vector<std::string> names_;
    std::map<const noise::module::Module*, std::string> module_to_name_;
    bool is_names_cached_;
};

#endif // UI_VIEW_CONTROLLER_MODULE_MANAGER_VIEW_CONTROLLER_H