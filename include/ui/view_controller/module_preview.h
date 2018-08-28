#ifndef UI_VIEW_CONTROLLER_MODULE_PREVIEW_H
#define UI_VIEW_CONTROLLER_MODULE_PREVIEW_H

#include "ui/view_controller/view.h"
#include "noise_gen/noise_module.h"

#include "output_gen/texture_generator.h"

#include <Magnum/GL/Texture.h>

class ModulePreview : public View
{
public:
    ModulePreview();

    virtual void update(NoiseModule& module);
    virtual void render() override;

private:
    Magnum::GL::Texture2D texture_;
    TextureGenerator generator_;
};

#endif