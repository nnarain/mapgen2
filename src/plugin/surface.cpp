#include "plugin/surface.h"

#include <Magnum/ImageView.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/TextureFormat.h>
#include <Corrade/Containers/ArrayView.h>

Surface::Surface(const Magnum::Vector2i& size) : Surface(size.x(), size.y())
{
}

Surface::Surface(std::size_t w, std::size_t h)
    : buffer_{}
    , width_{w}
    , height_{h}
{
    // resize for width x height x pixel size
    buffer_.resize(w * h * pixel_size);
}

void Surface::setColor(std::size_t x, std::size_t y, const Color& c)
{
    setColor(x, y, c.red, c.green, c.blue);
}

void Surface::setColor(std::size_t x, std::size_t y, uint8_t r, uint8_t g, uint8_t b)
{
    auto idx = (y * width_ * pixel_size) + (x * pixel_size);
    buffer_[idx + 0] = r;
    buffer_[idx + 1] = g;
    buffer_[idx + 2] = b;
}

void Surface::commit(Magnum::GL::Texture2D& texuture)
{
    // create an array view for this data
    Corrade::Containers::ArrayView<char> data_view{ (char*)&buffer_[0], buffer_.size() };
    // create an image2d view to bind to the texture
    Magnum::ImageView2D image{ Magnum::PixelFormat::RGB8Unorm, {(int)width_, (int)height_}, data_view };
    // bind image to texture
    texuture
        .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear)
        .setStorage(1, Magnum::GL::TextureFormat::RGB8, image.size())
        .setSubImage(0, {}, image);
}

const std::vector<uint8_t>& Surface::getBuffer() const
{
    return buffer_;
}

std::size_t Surface::getWidth() const noexcept
{
    return width_;
}

std::size_t Surface::getHeight() const noexcept
{
    return height_;
}