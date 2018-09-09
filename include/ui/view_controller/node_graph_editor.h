#ifndef UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H
#define UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H

#include "ui/view_controller/tab_renderer.h"
#include "ui/view_controller/module_manager_controller.h"

#include "addons/imguinodegrapheditor/imguinodegrapheditor.h"

class NodeGraphEditorTab : public TabRenderer
{
public:
    // TODO: scoped
    enum NodeTypes
    {
        BILLOW = NoiseModule::Type::Billow,
        PERLIN = NoiseModule::Type::Perlin,
        SELECT = NoiseModule::Type::Select,
        OUTPUT,
        NODE_TYPE_COUNT
    };

    NodeGraphEditorTab(ModuleManager& manager);
    virtual ~NodeGraphEditorTab();

    virtual void renderTab() override;

private:

    static ImGui::Node* nodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& nge);
    static void linkCallback(const ImGui::NodeLink& link, ImGui::NodeGraphEditor::LinkState state, ImGui::NodeGraphEditor& nge);

    ModuleManagerController manager_;
    ImGui::NodeGraphEditor nge;
};

#endif // UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H