#include "mapgen_application.h"
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GLContext.h>
#include <imgui.h>

using namespace Magnum;

MapGenApplication::MapGenApplication(const Arguments &arguments) 
    : Platform::Application{ arguments, Configuration{}.setTitle("MapGen").setSize({1280, 960}) }
    , module_manager_{}
    , ui_{module_manager_}
    , frame_timer_{16}
{
    using namespace Math::Literals;
    GL::Renderer::setClearColor(0xFFFFFF_rgbf);

    // TODO: remove
    module_manager_.create("noise1", NoiseModule::Type::Perlin);
    module_manager_.create("noise2", NoiseModule::Type::Select);
}

void MapGenApplication::drawEvent()
{
    frame_timer_.start();

    // Clear the window
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
    // new imgui frame
    imgui_.newFrame(windowSize(), GL::defaultFramebuffer.viewport().size());

    ui_.render();

    ImGui::ShowDemoWindow(nullptr);

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