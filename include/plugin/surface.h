#ifndef PLUGIN_SURFACE_H
#define PLUGIN_SURFACE_H

#include <noiseutils/noiseutils.h>

#include <vector>

/**
 * Drawing surface for user generators
*/
class Surface
{
public:
    using Color = noise::utils::Color;

    Surface(std::size_t w, std::size_t h);
    ~Surface() = default;

    /**
        Set the color at the (x,y) pixel
    */
    void setColor(std::size_t x, std::size_t y, const Color& c);
    /**
        Set the color at the (x,y) pixel
    */
    void setColor(std::size_t x, std::size_t y, uint8_t r, uint8_t g, uint8_t b);

    /**
        Get the underlying color buffer
    */
    const std::vector<uint8_t>& getBuffer() const;

    /**
        Get width
    */
    std::size_t getWidth() const noexcept;
    /**
        Get height
    */
    std::size_t getHeight() const noexcept;
private:
    static constexpr std::size_t pixel_size = 3;

    std::vector<uint8_t> buffer_;
    const std::size_t width_;
    const std::size_t height_;
};

#endif //