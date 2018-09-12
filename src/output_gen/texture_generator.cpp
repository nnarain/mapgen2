#include "output_gen/texture_generator.h"

#include <Magnum/GL/TextureFormat.h>
#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/ArrayView.h>
#include <noiseutils/noiseutils.h>

using namespace Magnum;

void TextureGenerator::generate(GL::Texture2D& output_texture, int w, int h, NoiseModule& noise)
{
    auto& module = noise.getModule();

    // generate the noise map
    utils::NoiseMap height_map;
    utils::NoiseMapBuilderPlane builder;
    builder.SetSourceModule(module);
    builder.SetDestNoiseMap(height_map);
    builder.SetDestSize(w, h);
    builder.SetBounds(2.0, 6.0, 1.0, 5.0);
    builder.Build();

    // render the noise map into an image
    utils::Image rendered_image;
    renderer_.SetSourceNoiseMap(height_map);
    renderer_.SetDestImage(rendered_image);
    renderer_.Render();

    // underlying color buffer for image
    auto* data = rendered_image.GetSlabPtr();
    auto data_size = static_cast<std::size_t>(w * h) * 4;
    Corrade::Containers::ArrayView<char> data_view{ (char*)data, data_size };

    
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            auto idx = ((y * w) + x) * 4;
            
            auto r = data_view[idx + 3];
            auto g = data_view[idx + 2];
            auto b = data_view[idx + 1];
            auto a = data_view[idx + 0];

            data_view[idx + 0] = r;
            data_view[idx + 1] = g;
            data_view[idx + 2] = b;
            data_view[idx + 3] = a;
        }
    }
    
    // create a magnum RGBA8 uint image with the rendered image data and bind it to the texture
    ImageView2D image{ PixelFormat::RGBA8Unorm, {w, h}, data_view };
    output_texture
        .setWrapping(GL::SamplerWrapping::ClampToEdge)
        .setMagnificationFilter(GL::SamplerFilter::Linear)
        .setMinificationFilter(GL::SamplerFilter::Linear)
        .setStorage(1, GL::TextureFormat::RGBA8, image.size())
        .setSubImage(0, {}, image);
}

utils::RendererImage& TextureGenerator::getRenderer() noexcept
{
    return renderer_;
}

