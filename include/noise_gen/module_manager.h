#ifndef NOISE_GEN_MODULE_MANAGER_H
#define NOISE_GEN_MODULE_MANAGER_H

#include "noise_gen/noise_module.h"

#include <map>
#include <memory>
#include <string>
#include <functional>

class ModuleManager
{
public:
    ModuleManager();

    void create(const std::string&, NoiseModule::Type);
    void remove(const std::string&);
    void rename(const std::string& current_name, const std::string& new_name);
    NoiseModule::Ptr& get(const std::string& name);
    bool has(const std::string& name) const;

    void forEach(std::function<void(const std::string&, NoiseModule&)> fn);
    
    void setSeed(int);
    
    std::size_t size() const noexcept;

private:
    std::map<std::string, NoiseModule::Ptr> modules_;
};

#endif // NOISE_GEN_MODULE_MANAGER_H