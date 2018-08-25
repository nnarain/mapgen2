#include "output_gen/texture_generator.h"

#include <noiseutils/noiseutils.h>

using namespace Magnum;

void TextureGenerator::generate(GL::Texture2D& output_texture, NoiseModule& noise)
{
    auto& module = *noise.getModule();

    auto size = output_texture.imageSize(0);
    auto w = size.x();
    auto h = size.y();

    // generate the noise map
    utils::NoiseMap height_map;
    utils::NoiseMapBuilderPlane builder;
    builder.SetSourceModule(module);
    builder.SetDestNoiseMap(height_map);
    builder.SetDestSize(w, h);
    builder.SetBounds(2.0, 6.0, 1.0, 5.0);
    builder.Build();

    // render the noise map into an image
    utils::RendererImage renderer;
    utils::Image rendered_image;
    renderer.SetSourceNoiseMap(height_map);
    renderer.SetDestImage(rendered_image);

    // underlying color buffer for image
    auto* data = rendered_image.GetSlabPtr();
    auto data_size = static_cast<std::size_t>(rendered_image.GetWidth() * rendered_image.GetHeight());

    // wrap image buffer in a corrade container
    Corrade::Containers::ArrayView<utils::Color> data_view{ data, data_size };

    // create a magnum RGBA8 uint image with the rendered image data and bind it to the texture
    ImageView2D image{ PixelFormat::RGBA8UI, {w, h}, data_view };
    output_texture.setSubImage(0, {}, image);
}

