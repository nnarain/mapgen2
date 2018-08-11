#include <noise/noise.h>
#include <noiseutils/noiseutils.h>

using namespace noise;


int main(int argc, char const *argv[])
{
    module::RidgedMulti mountain_terrain;

    module::Billow base_flat_terrain;
    base_flat_terrain.SetFrequency(2.0);
    
    module::ScaleBias flat_terrain;
    flat_terrain.SetSourceModule(0, base_flat_terrain);
    flat_terrain.SetScale(1.0 / 8.0);
    flat_terrain.SetBias(-0.75);

    module::Perlin terrain_type;
    terrain_type.SetFrequency(0.5);
    terrain_type.SetPersistence(0.25);

    module::Select terrain_selector;
    terrain_selector.SetSourceModule(0, flat_terrain);
    terrain_selector.SetSourceModule(1, mountain_terrain);
    terrain_selector.SetControlModule(terrain_type);
    terrain_selector.SetBounds(0, 1000);
    terrain_selector.SetEdgeFalloff(0.125);

    module::Turbulence final_terrain;
    final_terrain.SetSourceModule(0, terrain_selector);
    final_terrain.SetFrequency(4);
    final_terrain.SetPower(0.125);

    utils::NoiseMap height_map;
    utils::NoiseMapBuilderPlane height_map_builder;

    height_map_builder.SetSourceModule(terrain_selector);
    height_map_builder.SetDestNoiseMap(height_map);
    height_map_builder.SetDestSize(256, 256);
    height_map_builder.SetBounds(6, 10, 1, 5);
    height_map_builder.Build();

    utils::RendererImage renderer;
    utils::Image image;

    renderer.SetSourceNoiseMap(height_map);
    renderer.SetDestImage(image);
    renderer.ClearGradient();
    renderer.AddGradientPoint(-1.00, utils::Color(32, 160, 0, 255)); // grass
    renderer.AddGradientPoint(-0.25, utils::Color(224, 224, 0, 255)); // dirt
    renderer.AddGradientPoint(0.25, utils::Color(128, 128, 128, 255)); // rock
    renderer.AddGradientPoint(1.00, utils::Color(255, 255, 255, 255)); // snow
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(2.0);
    renderer.Render();

    utils::WriterBMP writer;
    writer.SetSourceImage(image);
    writer.SetDestFilename("tutorial.bmp");
    writer.WriteDestFile();

    return 0;
}
