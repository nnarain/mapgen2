#include "generators/simple_terrain.h"
#include "plugin/surface.h"
#include "plugin/utils.h"

#include <iostream>


SimpleTerrainGenerator::SimpleTerrainGenerator()
{
}

std::map<std::string, PluginBase::Parameter> SimpleTerrainGenerator::initializeParameters()
{
    return {
        {"color", noise::utils::Color()}
    };
}

void SimpleTerrainGenerator::update(const ParameterMap& params, NoiseMapManager& maps)
{
    height_module_ = maps["terrain"]->getOutputModule();

    color_ = boost::get<noise::utils::Color>(params.at("color"));
}

void SimpleTerrainGenerator::generate(Magnum::GL::Texture2D& target, Magnum::Vector2i& size)
{
    // build height map
    auto height_map = mapgen::utils::moduleToHeightMap(height_module_, size.x(), size.y());

    Surface surface(size);

    for (auto x = 0; x < size.x(); ++x)
    {
        for (auto y = 0; y < size.y(); ++y)
        {
            auto height = height_map.GetValue(x, y);
            height = mapgen::utils::commonRange(height);

            auto c = mapgen::utils::mulc(color_, height);
            surface.setColor(x, y, c);
        }
    }

    surface.commit(target);
}