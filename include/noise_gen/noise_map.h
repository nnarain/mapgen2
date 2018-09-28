#ifndef NOISE_GEN_NOISE_MAP_H
#define NOISE_GEN_NOISE_MAP_H

#include "noise_gen/noise_module.h"

#include <map>
#include <memory>
#include <string>
#include <functional>

class NoiseMap
{
public:
    NoiseMap();

    void add(const std::string&, NoiseModule::Type);
    void remove(const std::string&);
    void rename(const std::string& current_name, const std::string& new_name);

    void setOutputModule(NoiseModule::Ref ref);

    NoiseModule::Ptr& NoiseMap::createModuleWithUniqueName(NoiseModule::Type type);

    NoiseModule::Ptr& get(const std::string& name);
    bool has(const std::string& name) const;

    void forEach(std::function<void(const std::string&, NoiseModule&)> fn);
    
    void setSeed(int) noexcept;
    
    std::size_t size() const noexcept;

private:
    std::map<std::string, NoiseModule::Ptr> modules_;
    NoiseModule::Ref output_;
};

#endif // NOISE_GEN_NOISE_MAP_H