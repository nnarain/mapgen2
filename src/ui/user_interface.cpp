#include "ui/user_interface.h"

#include <imgui.h>

#include <iostream>

static constexpr ImGuiWindowFlags TAB_WINDOW_FLAGS = ImGuiWindowFlags_NoScrollbar |
                                                    ImGuiWindowFlags_NoCollapse |
                                                    ImGuiWindowFlags_NoTitleBar;

UserInterface::UserInterface()
{
    
}

UserInterface::~UserInterface()
{
}

void UserInterface::initialize()
{
    ImGui::TabWindow::SetWindowContentDrawerCallback(&UserInterface::tabContentProvider);
}

void UserInterface::render()
{
    renderMenu();
    renderTabs();
}

void UserInterface::renderTabs()
{
    static bool open = true;

    auto& window_size = ImGui::GetIO().DisplaySize;
    auto cursor_pos = ImGui::GetCursorPos();
    
    auto width = window_size.x;
    auto height = window_size.y - cursor_pos.y;

    ImGui::SetNextWindowPos({ 0, cursor_pos.y - ImGui::GetStyle().WindowPadding.y });
    ImGui::SetNextWindowSize({ width, height });

    if (ImGui::Begin("##TabWindow", &open, TAB_WINDOW_FLAGS))
    {
        static ImGui::TabWindow tab_window;
        if (!tab_window.isInited())
        {
            for (auto& pair : tabs_)
            {
                auto& name = pair.first;
                auto& tab = pair.second;

                tab_window.addTabLabel(name.c_str(), nullptr, false, false, tab.get());
            }
        }

        tab_window.render();
    }
    ImGui::End();
}

void UserInterface::tabContentProvider(ImGui::TabWindow::TabLabel* tab, ImGui::TabWindow& parent, void* user_ptr)
{
    if (tab->userPtr != nullptr)
    {
        auto ptr = static_cast<TabRenderer*>(tab->userPtr);
        ptr->renderTab();
    }
}

void UserInterface::renderMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Views"))
        {
            for (auto& pair : views_)
            {
                auto visible = pair.second->isVisible();
                ImGui::MenuItem(pair.first.c_str(), nullptr, &visible);
                pair.second->setVisible(visible);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    for (auto& pair : views_)
    {
        pair.second->draw();
    }
}