#include "generators/simple_terrain.h"

SimpleTerrainGenerator::SimpleTerrainGenerator()
    : r{0}, g{0}, b{0}
{
}

std::map<std::string, PluginBase::Parameter> SimpleTerrainGenerator::initializeParameters()
{
    return {
        {"r", 0},
        {"g", 0},
        {"b", 0}
    };
}

void SimpleTerrainGenerator::update(const ParameterMap& params)
{
    r = boost::get<int>(params.at("r"));
    g = boost::get<int>(params.at("g"));
    b = boost::get<int>(params.at("b"));
}

void SimpleTerrainGenerator::generate(Magnum::GL::Texture2D& target, Magnum::Vector2i& size)
{
    Surface surface(size);

    for (auto x = 0; x < size.x(); ++x)
    {
        for (auto y = 0; y < size.y(); ++y)
        {
            surface.setColor(x, y, r, g, b);
        }
    }

    surface.commit(target);
}