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

void SimpleTerrainGenerator::generate(Magnum::GL::Texture2D& target, Magnum::Vector2i& size)
{
    Surface surface(size);

    for (auto x = 0; x < size.x(); ++x)
    {
        for (auto y = 0; y < size.y(); ++y)
        {
            surface.setColor(x, y, 255, 0, 0);
        }
    }

    surface.commit(target);
}