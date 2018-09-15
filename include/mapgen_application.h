
#include "ui/user_interface.h"
#include "ui/view_controller/module_manager_controller.h"
#include "output_gen/output_config.h"
#include "frame_timer.h"

#include <Magnum/Platform/Sdl2Application.h>

#include <MagnumImGui.h>

class MapGenApplication : public Magnum::Platform::Application
{
public:
    MapGenApplication(const Arguments &arguments);

private:
    void drawEvent() override;
    void viewportEvent(const Magnum::Vector2i& size) override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mouseScrollEvent(MouseScrollEvent& event) override;
    void textInputEvent(TextInputEvent& event) override;

    ModuleManager module_manager_;
    ModuleManagerController module_manager_controller_;
    OutputConfig output_configuration_;
    UserInterface ui_;

    MagnumImGui imgui_;

    FrameTimer frame_timer_;
};