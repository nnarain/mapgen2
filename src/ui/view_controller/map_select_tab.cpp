#include "ui/view_controller/map_select_tab.h"

#include <imgui.h>

MapSelectTab::MapSelectTab(NoiseMapManager& manager)
    : manager_{manager}
{
    
}

void MapSelectTab::renderTab()
{
    ImGui::BeginChild("left pane", ImVec2(150, 0), true);
    {
        // map selection
        manager_.forEach([this](const std::string& name, NoiseMap&) {
            if (ImGui::Selectable(name.c_str(), name == selected_))
            {
                selected_ = name;
                on_map_event_(MapEvent::Changed, name);
            }
        });
        if (ImGui::Button("Create"))
        {
            static int count = 0;
            std::string name = "map" + std::to_string(count++);

            manager_.createNoiseMap(name);

            on_map_event_(MapEvent::Created, name);
        }
        /*


        if (ImGui::Button("Remove"))
        {
            //on_map_event_(MapEvent::Removed, "default");

        }
        */
    }
    ImGui::EndChild();
}

void MapSelectTab::connect(std::function<void(MapEvent, std::string)> fn)
{
    on_map_event_.connect(fn);
}