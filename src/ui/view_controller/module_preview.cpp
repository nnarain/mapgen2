#include "ui/view_controller/module_preview.h"

#include <Magnum/GL/TextureFormat.h>

#include <MagnumImGui.h>
#include <imgui.h>

using namespace Magnum;

ModulePreview::ModulePreview(const Vector2& size, Magnum::Vector2 texture_size)
    : texture_size_{texture_size}
    , render_size_{size}
    , use_terrain_gradient_{false}
    , light_enabled_{false}
{

}

void ModulePreview::update(NoiseModule& module)
{
    if (module.isValid())
    {
        generator_.generate(texture_, texture_size_.x(), texture_size_.y(), module);
    }
}

void ModulePreview::render()
{
    ImGui::Image(texture_, { render_size_.x(), render_size_.y() });
}

bool ModulePreview::renderParameters()
{
    auto& renderer = generator_.getRenderer();

    bool updated = false;

    if (ImGui::Checkbox("terrain gradient", &use_terrain_gradient_))
    {
        updated = true;

        if (use_terrain_gradient_)
        {
            renderer.BuildTerrainGradient();
        }
        else
        {
            renderer.BuildGrayscaleGradient();
        }
    }

    if (ImGui::Checkbox("enable light", &light_enabled_))
    {
        updated = true;
        renderer.EnableLight(light_enabled_);
    }

    return updated;
}

void ModulePreview::setSize(const Vector2& size) noexcept
{
    render_size_ = size;
}

Vector2& ModulePreview::getSize() noexcept
{
    return render_size_;
}