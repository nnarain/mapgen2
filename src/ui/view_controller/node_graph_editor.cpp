#include "ui/view_controller/node_graph_editor.h"
#include <addons/imguinodegrapheditor/imguinodegrapheditor.h>

#include <imgui.h>

NodeGraphEditorTab::NodeGraphEditorTab()
{
}

NodeGraphEditorTab::~NodeGraphEditorTab()
{
}

void NodeGraphEditorTab::renderTab()
{
    ImGui::TestNodeGraphEditor();
}