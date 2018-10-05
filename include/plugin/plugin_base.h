#ifndef PLUGIN_PLUGIN_BASE_H
#define PLUGIN_PLUGIN_BASE_H

#include "plugin/surface.h"

#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Vector2.h>

#include <boost/variant.hpp>

#include <map>
#include <string>

/**
    Base class for user defined generators
*/
class PluginBase
{
public:
    using Parameter = boost::variant<int, float, bool>;
    using ParameterMap = std::map<std::string, Parameter>;

    PluginBase() = default;
    ~PluginBase() = default;

    /**
        return parameters used by plugin
    */
    virtual std::map<std::string, Parameter> initializeParameters() = 0;

    /**
        Update plugin parameters
    */
    virtual void update(const ParameterMap& params) = 0;

    /**
        Generate the map into the provided texture
    */
    virtual void generate(Magnum::GL::Texture2D& target, Magnum::Vector2i& size) = 0;

private:
};

#endif