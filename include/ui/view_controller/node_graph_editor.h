#ifndef UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H
#define UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H

#include "ui/view_controller/tab_renderer.h"
#include "ui/view_controller/noise_map_controller.h"
#include "noise_gen/noise_map_manager.h"
#include "ui/events/map_event.h"

#include "addons/imguinodegrapheditor/imguinodegrapheditor.h"

#include <boost/signals2.hpp>

#include <string>
#include <memory>
#include <functional>

class NodeGraphEditorTab : public TabRenderer
{
public:
    using Ptr = std::unique_ptr<NodeGraphEditorTab>;

    // TODO: scoped
    enum NodeTypes
    {
        ABS          = NoiseModule::Type::Abs,
        ADD          = NoiseModule::Type::Add,
        BILLOW       = NoiseModule::Type::Billow,
        BLEND        = NoiseModule::Type::Blend,
        CACHE        = NoiseModule::Type::Cache,
        CHECKERBOARD = NoiseModule::Type::Checkerboard,
        CLAMP        = NoiseModule::Type::Clamp,
        CONST        = NoiseModule::Type::Const,
        CYLINDERS    = NoiseModule::Type::Cylinders,
        DISPLACE     = NoiseModule::Type::Displace,
        EXPONENT     = NoiseModule::Type::Exponent,
        INVERT       = NoiseModule::Type::Invert,
        MAX          = NoiseModule::Type::Max,
        MIN          = NoiseModule::Type::Min,
        MULTIPLY     = NoiseModule::Type::Multiply,
        PERLIN       = NoiseModule::Type::Perlin,
        POWER        = NoiseModule::Type::Power,
        SELECT       = NoiseModule::Type::Select,
        SPHERES      = NoiseModule::Type::Spheres,
        ROTATE_POINT = NoiseModule::Type::RotatePoint,
        SCALE_POINT  = NoiseModule::Type::ScalePoint,
        TRANSATE     = NoiseModule::Type::TranslatePoint,
        TURBULENCE   = NoiseModule::Type::Turbulence,
        VORONOI      = NoiseModule::Type::Voronoi,
        OUTPUT,
        NODE_TYPE_COUNT
    };

    NodeGraphEditorTab(NoiseMapManager& manager);
    virtual ~NodeGraphEditorTab();

    virtual void renderTab() override;

    void onMapEvent(MapEvent event, std::string map_name);

    void connect(std::function<void(NoiseModule::Ref)>);

private:
    void createNodeGraphEditor(const std::string& name, NoiseMap::Ptr& noisemap);
    void removeNodeGraphEditor(const std::string& name);
    void selectNodeGraphEditor(const std::string& name);

    static ImGui::Node* nodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& nge);
    static void linkCallback(const ImGui::NodeLink& link, ImGui::NodeGraphEditor::LinkState state, ImGui::NodeGraphEditor& nge);
    static void nodeCallback(ImGui::Node*& node, ImGui::NodeGraphEditor::NodeState state, ImGui::NodeGraphEditor& nge);

    NoiseMapManager& manager_;

    std::map<std::string, std::shared_ptr<ImGui::NodeGraphEditor>> editors_;
    std::weak_ptr<ImGui::NodeGraphEditor> current_editor_;
    
    boost::signals2::signal<void(NoiseModule::Ref)> on_output_changed_;
};

#endif // UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H