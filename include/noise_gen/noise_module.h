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

    enum class Type
    {
        Abs,
        Add,
        Billow,
        Blend,
        Cache,
        Checkerboard,
        Clamp,
        Const,
        Cylinders,
        Displace,
        Exponent,
        Invert,
        Max,
        Min,
        Multiply,
        Perlin,
        Power,
        RidgedMulti,
        RotatePoint,
        ScaleBias,
        ScalePoint,
        Select,
        Spheres,
        Turbulence,
        Voronoi
    };

    using ModuleVariant = boost::variant <
        noise::module::Abs,
        noise::module::Add,
        noise::module::Billow,
        noise::module::Blend,
        noise::module::Cache,
        noise::module::Checkerboard,
        noise::module::Clamp,
        noise::module::Const,
        noise::module::Cylinders,
        noise::module::Displace,
        noise::module::Exponent,
        noise::module::Invert,
        noise::module::Max,
        noise::module::Min,
        noise::module::Multiply,
        noise::module::Perlin,
        noise::module::Power,
        noise::module::RidgedMulti,
        noise::module::RotatePoint,
        noise::module::ScaleBias,
        noise::module::ScalePoint,
        noise::module::Select,
        noise::module::Spheres,
        noise::module::Turbulence,
        noise::module::Voronoi
    >;
    using ParameterVariant = boost::variant<
        int,
        float,
        bool,
        RangedInt,
        RangedFloat
    >;
    using ParameterMap = std::map<std::string, ParameterVariant>;
    using ParameterMapPtr = std::shared_ptr<ParameterMap>;



    NoiseModule(const std::string& name, Type type);

    void update();

    bool updateParameters(std::function<bool(const std::string& name, ParameterVariant&)>);

    noise::module::Module& getModule();

    ParameterMapPtr getParams();

    const std::string& getName() const;
    void setName(const std::string& name);

    Type getType() const;

    bool isValid() const;

    void setSourceModule(int index, NoiseModule::Ptr module);
    NoiseModule::Ref getSourceModule(int index);
    int getSourceModuleCount();


private:

    bool validate();

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