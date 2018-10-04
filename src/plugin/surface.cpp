#include "plugin/surface.h"

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