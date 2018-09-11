#include "ui/view_controller/module_preview.h"

#include <Magnum/GL/TextureFormat.h>

#include <MagnumImGui.h>
#include <imgui.h>

using namespace Magnum;

ModulePreview::ModulePreview(const Vector2& size)
    : size_{size}
{

}

void ModulePreview::update(NoiseModule& module)
{
    if (module.isValid())
    {
        generator_.generate(texture_, 128, 128, module);
    }
}

void ModulePreview::render()
{
    ImGui::Image(texture_, { size_.x(), size_.y() });
}

void ModulePreview::setSize(const Vector2& size)
{
    size_ = size;
}

Vector2& ModulePreview::getSize() noexcept
{
    return size_;
}