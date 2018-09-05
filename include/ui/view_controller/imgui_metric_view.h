#ifndef UI_VIEW_CONTROLLER_IMGUI_METRICS_VIEW_H
#define UI_VIEW_CONTROLLER_IMGUI_METRICS_VIEW_H

#include "ui/view_controller/view.h"
#include <imgui.h>

class ImGuiMetricsView : public View
{
protected:
    void render() override
    {
        ImGui::ShowMetricsWindow();
    }
};

#endif // UI_VIEW_CONTROLLER_IMGUI_METRICS_VIEW_H