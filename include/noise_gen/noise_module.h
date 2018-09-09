#ifndef NOISE_GEN_NOISE_MODULE_H
#define NOISE_GEN_NOISE_MODULE_H

#include "noise_gen/parameter.h"

// TODO: All client must compile these noise modules...
#include <noise/module/module.h>

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <boost/variant.hpp>

class NoiseModule
{
public:
    using Ptr = std::shared_ptr<NoiseModule>;
    using Ref = std::weak_ptr<NoiseModule>;

    using ModuleVariant = boost::variant<
        noise::module::Billow,
        noise::module::Perlin,
        noise::module::RidgedMulti,
        noise::module::ScaleBias,
        noise::module::Select
    >;
    using ParameterVariant = boost::variant<int, float, RangedInt, RangedFloat, NoiseModule::Ref>;
    using ParameterMap = std::map<std::string, ParameterVariant>;
    using ParameterMapPtr = std::shared_ptr<ParameterMap>;

    enum class Type
    {
        Billow,
        Perlin,
        RidgedMulti,
        ScaleBias,
        Select
    };

    NoiseModule(const std::string& name, Type type);

    void update();

    noise::module::Module& getModule();

    ParameterMapPtr getParams();

    const std::string& getName() const;
    void setName(const std::string& name);

    Type getType() const;

    bool isValid() const;

    void setSourceModule(int index, NoiseModule::Ptr& module);
    NoiseModule::Ref getSourceModule(int index);
    int getSourceModuleCount();


private:

    bool validate();
    int getActualSourceCount();

    ModuleVariant module_base_;
    noise::module::Module& module_;
    std::vector<NoiseModule::Ref> source_refs_;
    
    std::string name_;
    Type type_;
    ParameterMapPtr parameter_map_;
    bool is_valid_;
    int actual_source_count_;
};

#endif // NOISE_GEN_NOISE_MODULE_H