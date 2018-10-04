#ifndef PLUGIN_PLUGIN_BASE_H
#define PLUGIN_PLUGIN_BASE_H

#include "plugin/surface.h"

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

    PluginBase() = default;
    ~PluginBase() = default;

    virtual std::map<std::string, Parameter> initializeParameters() = 0;
    virtual void generate(Surface& target) = 0;

private:
};

#endif