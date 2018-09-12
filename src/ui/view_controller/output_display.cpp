#include "ui/view_controller/output_display.h"

#include <imgui.h>

OutputDisplayTab::OutputDisplayTab(ModuleManagerController& manager)
    : manager_{manager}
    , preview_{ {256, 256} }
    , update_required_{false}
{
    manager_.addOutputChangedObserver(std::bind(&OutputDisplayTab::onOutputChanged, this, std::placeholders::_1));
}

OutputDisplayTab::~OutputDisplayTab()
{
}

void OutputDisplayTab::renderTab()
{
    constexpr float width_percent = 0.25f;

    // update output if it changed
    if (update_required_)
    {
        update_required_ = false;
        
        if (auto module = output_module_.lock())
        {
            preview_.update(*module);
        }
    }

    auto window_size = ImGui::GetWindowSize();

    ImGui::BeginChild("parameter panel", { window_size.x * width_percent, window_size.y }, true);
    {
        if (ImGui::CollapsingHeader("Renderer Configuration", ImGuiTreeNodeFlags_DefaultOpen))
        {
            update_required_ = preview_.renderParameters();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    preview_.render();
}

void OutputDisplayTab::onOutputChanged(NoiseModule::Ref ref)
{
    output_module_ = ref;
    update_required_ = true;
}