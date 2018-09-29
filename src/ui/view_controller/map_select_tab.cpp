#include "ui/view_controller/map_select_tab.h"

#include <imgui.h>

MapSelectTab::MapSelectTab(NoiseMapManager& manager)
    : manager_{manager}
{
    
}

void MapSelectTab::renderTab()
{
    static constexpr const char* id = "map_select";

    // remove the selected noise map
    if (ImGui::Button("remove"))
    {
        if (!selected_.empty())
        {
            on_map_event_(MapEvent::Removed, selected_);

            manager_.removeNoiseMap(selected_);
            selected_.clear();
        }
    }

    // draw all noise maps that can be selected
    ImGui::BeginChild(id, ImVec2(150, 0), true);
    {
        // map selection
        manager_.forEach([this](const std::string& name, NoiseMap&) {
            if (ImGui::Selectable(name.c_str(), name == selected_))
            {
                selected_ = name;
                on_map_event_(MapEvent::Changed, name);
            }
        });
    }
    ImGui::EndChild();

    if (ImGui::BeginPopupContextItem(id))
    {
        static char buf[32] = "map";

        ImGui::InputText("name", buf, IM_ARRAYSIZE(buf));
        ImGui::SameLine();
        if (ImGui::Button("add"))
        {
            const std::string name{ buf };
            manager_.createNoiseMap(name);
            on_map_event_(MapEvent::Created, name);

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void MapSelectTab::connect(std::function<void(MapEvent, std::string)> fn)
{
    on_map_event_.connect(fn);
}