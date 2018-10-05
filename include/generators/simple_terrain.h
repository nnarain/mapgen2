#ifndef GENERATORS_SIMPLE_TERRAIN_H
#define GENERATORS_SIMPLE_TERRAIN_H

#include "plugin/plugin_base.h"

class SimpleTerrainGenerator : public PluginBase
{
public:
    SimpleTerrainGenerator();
    virtual ~SimpleTerrainGenerator() = default;

    virtual std::map<std::string, Parameter> initializeParameters() override;
    virtual void generate(Magnum::GL::Texture2D& target, Magnum::Vector2i& size) override;

private:
};

#endif