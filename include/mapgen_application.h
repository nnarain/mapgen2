
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

    MagnumImGui imgui;
};