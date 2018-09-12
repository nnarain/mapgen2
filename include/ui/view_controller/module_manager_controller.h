#ifndef UI_VIEW_CONTROLLER_MODULE_MANAGER_VIEW_CONTROLLER_H
#define UI_VIEW_CONTROLLER_MODULE_MANAGER_VIEW_CONTROLLER_H

#include "noise_gen/module_manager.h"

#include <boost/signals2.hpp>

#include <string>
#include <vector>
#include <map>
#include <functional>

/**
    Interface to ModuleManager
*/
class ModuleManagerController
{
    using OutputChangedSignature = void(NoiseModule::Ref);

public:
    ModuleManagerController(ModuleManager& manager);

    NoiseModule::Ptr& get(const std::string& name);
    bool has(const std::string& name);

    void createModule(const std::string& name, NoiseModule::Type type);
    void removeModule(const std::string& name);

    NoiseModule::Ptr& createModuleWithUniqueName(NoiseModule::Type type);
    
    void renameModule(const std::string current_name, const std::string new_name);

    void setOutputModule(const NoiseModule::Ref output_ref);
    void addOutputChangedObserver(std::function<OutputChangedSignature>);

    const std::vector<std::string>& getModuleNames();

    const std::size_t size() const noexcept;

private:
    ModuleManager& manager_;

    std::vector<std::string> names_;
    bool is_names_cached_;

    NoiseModule::Ref output_ref_;

    boost::signals2::signal<OutputChangedSignature> output_changed_;
};

#endif // UI_VIEW_CONTROLLER_MODULE_MANAGER_VIEW_CONTROLLER_H