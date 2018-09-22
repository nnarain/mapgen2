#ifndef UI_VIEW_CONTROLLER_TEST_VIEW_H
#define UI_VIEW_CONTROLLER_TEST_VIEW_H

#include "ui/view_controller/view.h"
#include "ui/view_controller/module_manager_controller.h"

#include <addons/imguinodegrapheditor/imguinodegrapheditor.h>
#include <addons/imguitabwindow/imguitabwindow.h>
#include <imgui_color_gradient.h>

#include <functional>

class TestView : public View
{
public:
    TestView()
    {
    }

protected:
    void render() override
    {
        ///*
        ImGui::Begin("Testing");
        {
            /*
            static ImGradientMark *dragging_mark_ = nullptr;
            static ImGradientMark *selected_mark_ = nullptr;

            ImGui::GradientEditor(&gradient_, dragging_mark_, selected_mark_);
        
            */

            static int e = 0;
            ImGui::RadioButton("test a", &e, 0); ImGui::SameLine();
            ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
            ImGui::RadioButton("radio c", &e, 2);
        }
        ImGui::End();
        //*/

    }

private:
    ImGradient gradient_;

};

#endif //