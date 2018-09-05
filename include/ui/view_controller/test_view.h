#ifndef UI_VIEW_CONTROLLER_TEST_VIEW_H
#define UI_VIEW_CONTROLLER_TEST_VIEW_H

#include "ui/view_controller/view.h"
#include "ui/view_controller/module_manager_controller.h"

#include <addons/imguinodegrapheditor/imguinodegrapheditor.h>
#include <addons/imguitabwindow/imguitabwindow.h>

#include <functional>

class TestView : public View
{
public:
    TestView(ModuleManager& manager)
        : manager_{ manager }
    {
        ImGui::TabWindow::SetWindowContentDrawerCallback(&TestView::tabContentProvider);
    }

public:
    void nodeEditor()
    {
    //    if (ImGui::Begin("Node Editor"))
        {
            ImGui::TestNodeGraphEditor();
        }
    //    ImGui::End();
    }

protected:
    void render() override
    {
     //   nodeEditor();
        tabWindow();
    }

    void tabWindow()
    {
        static bool open = true;
        if (ImGui::Begin("Main", &open, ImVec2(400, 600), -1.f, ImGuiWindowFlags_NoScrollbar))
        {
            static ImGui::TabWindow tabWindow;
            if (!tabWindow.isInited())
            {
                static const char* tabNames[] = { "Render","Layers","Scene","World","Object","Constraints","Modifiers","Data","Material","Texture","Particle","Physics" };
                static const int numTabs = sizeof(tabNames) / sizeof(tabNames[0]);
                static const char* tabTooltips[numTabs] = { "Render Tab Tooltip","Layers Tab Tooltip","Scene Tab Tooltip","","Object Tab Tooltip","","","","","Tired to add tooltips...","" };
                for (int i = 0; i<numTabs; i++)
                {
                    tabWindow.addTabLabel(tabNames[i], tabTooltips[i], false, false, this); // see additional args to prevent a tab from closing and from dragging
                }
            }
            tabWindow.render(); // Must be called inside "its" window (and sets isInited() to false)
        }
        ImGui::End();
    }

    static void tabContentProvider(ImGui::TabWindow::TabLabel* tab, ImGui::TabWindow& parent, void* user_ptr)
    {
        auto* view = static_cast<TestView*>(user_ptr);
        view->nodeEditor();
    }

private:
    ModuleManagerController manager_;
};

#endif //