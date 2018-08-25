#include "ui/view_controller/module_preview.h"

using namespace Magnum;

ModulePreview::ModulePreview()
{
    texture_
        .setMagnificationFilter(GL::SamplerFilter::Linear)
        .setMinificationFilter(GL::SamplerFilter::Linear, GL::SamplerMipmap::Linear)
        .setWrapping(GL::SamplerWrapping::ClampToEdge);
}

void ModulePreview::update(NoiseModule& module)
{
    generator_.generate(texture_, module);
}

void ModulePreview::render()
{
    // TODO: Magnum Imgui::Image support
}