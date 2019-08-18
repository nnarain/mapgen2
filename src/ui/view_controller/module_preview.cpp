#include "ui/view_controller/module_preview.h"

#include <Magnum/GL/TextureFormat.h>

#include <MagnumImGui.h>
#include <imgui.h>

#include <iostream>

using namespace Magnum;

ModulePreview::ModulePreview(const Vector2& size, Magnum::Vector2 texture_size)
    : texture_size_{texture_size}
    , render_size_{size}
    , gradient_select_{0}
    , show_gradient_editor_{false}
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

void ModulePreview::renderParameters()
{
    auto& renderer = generator_.getRenderer();

    bool updated = false;

    ImGui::Text("Gradient:");
    bool gradient_updated = false;
    gradient_updated = ImGui::RadioButton("grayscale", &gradient_select_, 0) || gradient_updated;
    ImGui::SameLine();
    gradient_updated = ImGui::RadioButton("terrain", &gradient_select_, 1) || gradient_updated;
    ImGui::SameLine();
    gradient_updated = ImGui::RadioButton("custom", &gradient_select_, 2) || gradient_updated;

    if (gradient_updated)
    {
        updated = true;

        switch (gradient_select_)
        {
        case 0:
            renderer.BuildGrayscaleGradient();
            break;
        case 1:
            renderer.BuildTerrainGradient();
            break;
        case 2:
            renderer.ClearGradient();
            renderer.AddGradientPoint(0.0, noise::utils::Color(0, 0, 0, 0));
            renderer.AddGradientPoint(1.0, noise::utils::Color(1, 1, 1, 1));
            break;
        default:
            break;
        }
    }

    if (gradient_select_ == 2)
    {
        updated = customGradient(renderer);
    }

    ImGui::Text("Lighting:");

    if (ImGui::Checkbox("enable light", &light_enabled_))
    {
        updated = true;
        renderer.EnableLight(light_enabled_);
    }

    auto brightness = (float)renderer.GetLightBrightness();
    if (ImGui::DragFloat("brightness", &brightness, 0.01f, 0, 10))
    {
        updated = true;
        renderer.SetLightBrightness(brightness);
    }

    auto azimuth = (float)renderer.GetLightAzimuth();
    if (ImGui::DragFloat("light azimuth", &azimuth, 0.5f, 0, 360))
    {
        updated = true;
        renderer.SetLightAzimuth(azimuth);
    }

    auto light_elevation = (float)renderer.GetLightElev();
    if (ImGui::DragFloat("light elev", &light_elevation, 0.5f, 0, 90))
    {
        updated = true;
        renderer.SetLightElev(light_elevation);
    }

    auto light_intensity = (float)renderer.GetLightIntensity();
    if (ImGui::DragFloat("light intens", &light_intensity, 0.1f, 0, 10))
    {
        updated = true;
        renderer.SetLightIntensity(light_intensity);
    }

    if (updated)
    {
        generator_.renderImage(texture_, texture_size_.x(), texture_size_.y());
    }
}

bool ModulePreview::customGradient(noise::utils::RendererImage& renderer)
{
    if (ImGui::Button("edit"))
    {
        show_gradient_editor_ = !show_gradient_editor_;
    }

    bool updated = false;

    if (show_gradient_editor_)
    {
        if (gradient_editor_.render())
        {
            updated = true;

            renderer.ClearGradient();

            auto marks = gradient_editor_.getColorMarks();
            for (auto& mark : marks)
            {
                auto r = (uint8_t)(mark->color[0] * 255.f);
                auto g = (uint8_t)(mark->color[1] * 255.f);
                auto b = (uint8_t)(mark->color[2] * 255.f);

                try
                {
                    renderer.AddGradientPoint(mark->position, noise::utils::Color{ r, g, b, 255 });
                }
                catch (noise::ExceptionInvalidParam&)
                {
                    // TODO: This try-catch really should not be here. Instead the gradient positions should never be invalid
                }
            }
        }
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