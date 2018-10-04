#include "generators/simple_terrain.h"

SimpleTerrainGenerator::SimpleTerrainGenerator()
{
}

std::map<std::string, PluginBase::Parameter> SimpleTerrainGenerator::initializeParameters()
{
    return {
        {"test", 0.01f}
    };
}

void SimpleTerrainGenerator::generate(Surface& surface)
{
    const auto width = surface.getWidth();
    const auto height = surface.getHeight();

    for (auto x = 0u; x < width; ++x)
    {
        for (auto y = 0u; y < height; ++y)
        {
            surface.setColor(x, y, 255, 255, 0);
        }
    }
}