#ifndef NOISE_GEN_NOISE_MODULE_H
#define NOISE_GEN_NOISE_MODULE_H

#include "noise_gen/parameter.h"

// TODO: All client must compile these noise modules...
#include <noise/module/module.h>

#include <memory>
#include <map>
#include <string>
#include <boost/variant.hpp>

class NoiseModule
{
public:
    using Ptr = std::unique_ptr<NoiseModule>;
    using ModulePtr = std::unique_ptr<noise::module::Module>;

    using ModuleVariant = boost::variant<
        noise::module::Perlin,
        noise::module::Select
    >;
    using ParameterVariant = boost::variant<int, float, RangedInt, RangedFloat, NoiseModule*>;
    using ParameterMap = std::map<std::string, ParameterVariant>;
    using ParameterMapPtr = std::shared_ptr<ParameterMap>;

    enum class Type
    {
        Perlin,
        Select
    };

    NoiseModule(const std::string& name, Type type);

    void update();
    void invalidateSources();

    ParameterMapPtr getParams();
    noise::module::Module& getModule();
    const std::string& getName() const;
    Type getType() const;

    bool isValid() const;

    void setSourceModule(int index, NoiseModule& module);
    int getSourceModuleCount();


private:
    ModuleVariant module_base_;
    noise::module::Module& module_;
    
    std::string name_;
    Type type_;
    ParameterMapPtr parameter_map_;
    bool is_valid_;
};

#endif // NOISE_GEN_NOISE_MODULE_H