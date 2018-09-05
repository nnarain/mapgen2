#include "ui/user_interface.h"

#include <imgui.h>
#include <addons/imguinodegrapheditor/imguinodegrapheditor.h>

UserInterface::UserInterface()
{

}

UserInterface::~UserInterface()
{
}

void UserInterface::render()
{
    renderMenu();

    for (auto& pair : views_)
    {
        pair.second->draw();
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
}