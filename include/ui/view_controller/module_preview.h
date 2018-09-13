#ifndef UI_VIEW_CONTROLLER_MODULE_PREVIEW_H
#define UI_VIEW_CONTROLLER_MODULE_PREVIEW_H

#include "ui/view_controller/view.h"
#include "noise_gen/noise_module.h"

#include "output_gen/texture_generator.h"

#include <Magnum/GL/Texture.h>

class ModulePreview : public View
{
public:
    ModulePreview(const Magnum::Vector2&, Magnum::Vector2 texture_size = Magnum::Vector2{128, 128});

    virtual void update(NoiseModule& module);
    virtual void render() override;
    bool renderParameters();

    void setSize(const Magnum::Vector2&) noexcept;
    Magnum::Vector2& getSize() noexcept;

private:
    Magnum::GL::Texture2D texture_;
    Magnum::Vector2 texture_size_;
    Magnum::Vector2 render_size_;

    TextureGenerator generator_;

    // parameter vars
    bool use_terrain_gradient_;
    bool light_enabled_;
};

#endif