#ifndef PLUGIN_UTILS_H
#define PLUGIN_UTILS_H

#include "noise_gen/noise_module.h"
#include <noiseutils/noiseutils.h>

namespace mapgen
{
    namespace utils
    {
        /**
        Remap a range
        */
        template<typename T>
        T range(T value, T from1, T to1, T from2, T to2)
        {
            return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
        }

        template<typename T>
        T lerp(T a, T b, T t)
        {
            return a + (b - a) * t;
        }

        template<typename T>
        T clamp(T v, T min, T max)
        {
            v = (v < min) ? min : v;
            v = (v > max) ? max : v;

            return v;
        }

        template<typename T>
        T commonRange(T value)
        {
            value = range(value, -1.0f, 1.0f, 0.0f, 1.0f);
            value = clamp(value, 0.0f, 1.0f);

            return value;
        }

        noise::utils::Color mulc(const noise::utils::Color& c, float t)
        {
            noise::utils::Color c2;
            c2.red = c.red * t;
            c2.green = c.green * t;
            c2.blue = c.blue * t;
            c2.alpha = c.alpha * t;

            return c2;
        }

        noise::utils::NoiseMap moduleToNoiseMap(NoiseModule::Ref ref, std::size_t w, std::size_t h)
        {
            noise::utils::NoiseMap map;

            if (auto module = ref.lock())
            {
                noise::utils::NoiseMapBuilderPlane builder;
                builder.SetSourceModule(module->getModule());
                builder.SetDestNoiseMap(map);
                builder.SetDestSize((int)w, (int)h);
                builder.SetBounds(2.0, 6.0, 1.0, 5.0);
                builder.Build();
            }

            return map;
        }
    }
} // mapgen


#endif