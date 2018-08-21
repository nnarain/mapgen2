#include "ui/user_interface.h"

#include <imgui.h>

#include <iostream>

static constexpr const char* MODULE_TYPES[] = {
    "perlin",
    "select"
};

UserInterface::UserInterface()
{

}

UserInterface::~UserInterface()
{
}

void UserInterface::render(ModuleManager& manager)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Editor"))
    {
        // module list
        static constexpr const char* module_select_id = "module_select";
        ImGui::BeginChild(module_select_id, ImVec2(150, 0), true);
        {
            manager.forEach([this](const std::string& name, NoiseModule& module)
            {
                //std::cout << "Listing " << name << std::endl;
                if (ImGui::Selectable(name.c_str(), selected_module_ == name))
                {
                    selected_module_ = name;
                }
            });
        }
        ImGui::EndChild();

        // Context menu for the module select pane
        if (ImGui::BeginPopupContextItem(module_select_id))
        {
            static char buf[32] = "module1";

            ImGui::Text("Module name:");
            ImGui::InputText("##edit", buf, IM_ARRAYSIZE(buf));

            ImGui::Text("Module Type:");
            static int type_select = 0;
            ImGui::Combo("", &type_select, MODULE_TYPES, IM_ARRAYSIZE(MODULE_TYPES));

            if (ImGui::Button("add"))
            {
                NoiseModule::Type type = static_cast<NoiseModule::Type>(type_select);
                std::string name(buf);

                if (!manager.has(name))
                {
                    manager.create(name, type);
                    std::strncpy(buf, "module1", IM_ARRAYSIZE(buf));
                }
                else
                {
                    std::cout << "Module with name, " << name << ", already exists" << std::endl;
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }
}
