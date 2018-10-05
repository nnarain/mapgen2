#include "generators/simple_terrain.h"
#include "plugin/surface.h"

SimpleTerrainGenerator::SimpleTerrainGenerator()
{
}

std::map<std::string, PluginBase::Parameter> SimpleTerrainGenerator::initializeParameters()
{
    return {

    };
}

void SimpleTerrainGenerator::update(const ParameterMap& params, NoiseMapManager& maps)
{
    height_module_ = maps["terrain"]->getOutputModule();
}

void SimpleTerrainGenerator::generate(Magnum::GL::Texture2D& target, Magnum::Vector2i& size)
{
    Surface surface(size);

    for (auto x = 0; x < size.x(); ++x)
    {
        for (auto y = 0; y < size.y(); ++y)
        {
            surface.setColor(x, y, 0, 255, 0);
        }
    }

    surface.commit(target);
}