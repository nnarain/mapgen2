#include "mapgen_application.h"
#include "ui/view_controller/imgui_demo_view.h"
#include "ui/view_controller/imgui_metric_view.h"
#include "ui/view_controller/node_graph_editor.h"
#include "ui/view_controller/output_config_tab.h"
#include "ui/view_controller/test_view.h"

#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GLContext.h>
#include <imgui.h>


using namespace Magnum;

MapGenApplication::MapGenApplication(const Arguments &arguments) 
    : Platform::Application{ arguments, Configuration{}.setTitle("MapGen").setSize({1280, 960}) }
    , module_manager_{}
    , module_manager_controller_{module_manager_}
    , ui_{}
    , frame_timer_{16}
{
    using namespace Math::Literals;
    GL::Renderer::setClearColor(0xFFFFFF_rgbf);

    ui_.initialize();

    ui_.addTab<NodeGraphEditorTab>("Editor", module_manager_controller_);
    ui_.addTab<OutputConfigTab>("Output Config", module_manager_controller_, output_configuration_);

    ui_.addView<ImGuiDemoView>("Demo", false);
    ui_.addView<ImGuiMetricsView>("Metrics", false);
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

void MapGenApplication::viewportEvent(const Vector2i &size)
{
    GL::defaultFramebuffer.setViewport({ {}, size });
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