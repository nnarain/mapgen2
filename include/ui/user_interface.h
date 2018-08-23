#ifndef UI_USER_INTERFACE_H
#define UI_USER_INTERFACE_H

#include "noise_gen/module_manager.h"
#include "ui/view_controller/module_manager_controller.h"
#include "ui/view_controller/editor_view.h"

#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Math/Vector2.h>
#include <MagnumImgui.h>

class UserInterface
{
public:
    UserInterface(ModuleManager& manager);
    ~UserInterface();

    void render();

private:
    ModuleManagerController manager_;
    EditorView editor_;
};

#endif // UI_USER_INTERFACE_H