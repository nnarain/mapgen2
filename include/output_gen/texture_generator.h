#ifndef OUTPUT_GEN_TEXTURE_GENERATOR_H
#define OUTPUT_GEN_TEXTURE_GENERATOR_H

#include "noise_gen/noise_module.h"

#include <Magnum/GL/Texture.h>
#include <Magnum/ImageView.h>
#include <noiseutils/noiseutils.h>

class TextureGenerator
{
public:

    void generate(Magnum::GL::Texture2D& out_texture, int w, int h, NoiseModule& noise);

private:
    
};

#endif