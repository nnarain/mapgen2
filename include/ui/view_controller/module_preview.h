#ifndef UI_VIEW_CONTROLLER_MODULE_PREVIEW_H
#define UI_VIEW_CONTROLLER_MODULE_PREVIEW_H

#include "ui/view_controller/view.h"
#include "noise_gen/noise_module.h"

#include "output_gen/texture_generator.h"

#include <Magnum/GL/Texture.h>

class ModulePreview : public View
{
public:
    ModulePreview(const Magnum::Vector2&);

    virtual void update(NoiseModule& module);
    virtual void render() override;

    void setSize(const Magnum::Vector2&);
    Magnum::Vector2& getSize() noexcept;

private:
    Magnum::Vector2 size_;
    Magnum::GL::Texture2D texture_;
    TextureGenerator generator_;
};

#endif