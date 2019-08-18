
#include "ui/view_controller/gradient_editor.h"

GradientEditor::GradientEditor() 
    : dragging_mark_{nullptr}
    , selected_mark_{nullptr}
{

}

bool GradientEditor::render()
{
    bool updated = false;

    ImGui::SetNextWindowSize({ 640, 640 }, ImGuiCond_FirstUseEver);

    ImGui::Begin("Gradient");
    {
        updated = ImGui::GradientEditor(&gradient_, dragging_mark_, selected_mark_);
    }
    ImGui::End();

    return updated;
}

std::list<ImGradientMark*>& GradientEditor::getColorMarks()
{
    return gradient_.getMarks();
}