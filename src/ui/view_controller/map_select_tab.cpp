#include "ui/view_controller/map_select_tab.h"

#include <imgui.h>

MapSelectTab::MapSelectTab()
{

}

void MapSelectTab::renderTab()
{
    ImGui::BeginChild("left pane", ImVec2(150, 0), true);
    {

    }
    ImGui::EndChild();
}