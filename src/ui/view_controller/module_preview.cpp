#include "ui/view_controller/module_preview.h"

#include <Magnum/GL/TextureFormat.h>

#include <MagnumImGui.h>
#include <imgui.h>

using namespace Magnum;

ModulePreview::ModulePreview()
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
    auto parent_width = ImGui::GetWindowWidth();
    auto parent_height = ImGui::GetWindowHeight() - ImGui::GetCursorPosY();
    ImGui::Image(texture_, { parent_width, parent_height });
}