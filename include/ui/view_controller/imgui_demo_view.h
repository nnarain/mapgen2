#ifndef UI_VIEW_CONTROLLER_IMGUI_DEMO_VIEW_H
#define UI_VIEW_CONTROLLER_IMGUI_DEMO_VIEW_H

#include "ui/view_controller/view.h"
#include <imgui.h>

class ImGuiDemoView : public View
{
protected:
    void render() override
    {
        ImGui::ShowDemoWindow();
    }
};

#endif // UI_VIEW_CONTROLLER_IMGUI_DEMO_VIEW_H