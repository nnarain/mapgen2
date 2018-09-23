
#include "ui/user_interface.h"
#include "ui/view_controller/noise_map_controller.h"
#include "frame_timer.h"

#include <Magnum/Platform/Sdl2Application.h>

#include <MagnumImGui.h>

class MapGenApplication : public Magnum::Platform::Application
{
public:
    MapGenApplication(const Arguments &arguments);

private:
    void drawEvent() override;
    void viewportEvent(ViewportEvent& event) override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mouseScrollEvent(MouseScrollEvent& event) override;
    void textInputEvent(TextInputEvent& event) override;

    NoiseMap module_manager_;
    NoiseMapController module_manager_controller_;
    UserInterface ui_;

    MagnumImGui imgui_;

    FrameTimer frame_timer_;
};