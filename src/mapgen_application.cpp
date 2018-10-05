#include "mapgen_application.h"
#include "ui/view_controller/imgui_demo_view.h"
#include "ui/view_controller/imgui_metric_view.h"
#include "ui/view_controller/node_graph_editor.h"
#include "ui/view_controller/output_config_tab.h"
#include "ui/view_controller/map_select_tab.h"
#include "ui/view_controller/user_display_tab.h"
#include "ui/view_controller/test_view.h"

#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GLContext.h>
#include <imgui.h>

#include <functional>

using namespace Magnum;

MapGenApplication::MapGenApplication(const Arguments &arguments) 
    : Platform::Application{ arguments, Configuration{}.setTitle("MapGen").setSize({1280, 960}) }
    , ui_{}
    , frame_timer_{16}
{
    using namespace Math::Literals;
    GL::Renderer::setClearColor(0xFFFFFF_rgbf);

    ui_.initialize();

    // Create tabs
    MapSelectTab::Ptr map_select_tab = std::make_unique<MapSelectTab>(noise_map_manager_);
    NodeGraphEditorTab::Ptr node_graph_editor = std::make_unique<NodeGraphEditorTab>(noise_map_manager_);
    OutputConfigTab::Ptr output_config_tab = std::make_unique<OutputConfigTab>(noise_map_manager_);
    UserDisplayTab::Ptr user_display_tab = std::make_unique<UserDisplayTab>(noise_map_manager_);

    // Connect signals
    map_select_tab->connect(std::bind(&NodeGraphEditorTab::onMapEvent, node_graph_editor.get(), std::placeholders::_1, std::placeholders::_2));
    map_select_tab->connect(std::bind(&OutputConfigTab::onMapEvent, output_config_tab.get(), std::placeholders::_1, std::placeholders::_2));
    map_select_tab->connect(std::bind(&UserDisplayTab::onMapEvent, user_display_tab.get(), std::placeholders::_1, std::placeholders::_2));

    generator_ = std::make_shared<SimpleTerrainGenerator>();
    user_display_tab->setUserGenerator(generator_);

    ui_.addTab("Select", std::move(map_select_tab));
    ui_.addTab("Editor", std::move(node_graph_editor));
    ui_.addTab("Config", std::move(output_config_tab));
    ui_.addTab("Display", std::move(user_display_tab));

    ui_.addView<ImGuiDemoView>("Demo", false);
    ui_.addView<ImGuiMetricsView>("Metrics", false);
    ui_.addView<TestView>("Testing", false);
}

void MapGenApplication::drawEvent()
{
    frame_timer_.start();

    // Clear the window
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
    // new imgui frame
    imgui_.newFrame(windowSize(), GL::defaultFramebuffer.viewport().size());

    ui_.render();

    // draw imgui
    imgui_.drawFrame();

    swapBuffers();
    redraw();
    
    frame_timer_.end();
}

void MapGenApplication::viewportEvent(ViewportEvent& event)
{
    GL::defaultFramebuffer.setViewport({ {}, event.windowSize() });
}

void MapGenApplication::keyPressEvent(KeyEvent &event)
{
    // exit if interacing with GUI
    if (imgui_.keyPressEvent(event))
        return;
}

void MapGenApplication::keyReleaseEvent(KeyEvent &event)
{
    // exit if interacing with GUI
    if (imgui_.keyReleaseEvent(event))
        return;
}

void MapGenApplication::mousePressEvent(MouseEvent &event)
{
    // exit if interacing with GUI
    if (imgui_.mousePressEvent(event))
        return;
}

void MapGenApplication::mouseReleaseEvent(MouseEvent &event)
{
    // exit if interacing with GUI
    if (imgui_.mouseReleaseEvent(event))
        return;
}

void MapGenApplication::mouseMoveEvent(MouseMoveEvent &event)
{
    // exit if interacing with GUI
    if (imgui_.mouseMoveEvent(event))
        return;
}

void MapGenApplication::mouseScrollEvent(MouseScrollEvent &event)
{
    // exit if interacing with GUI
    if (imgui_.mouseScrollEvent(event))
        return;
}

void MapGenApplication::textInputEvent(TextInputEvent &event)
{
    // exit if interacing with GUI
    if (imgui_.textInputEvent(event))
        return;
}

/* main() function implementation */
MAGNUM_APPLICATION_MAIN(MapGenApplication)