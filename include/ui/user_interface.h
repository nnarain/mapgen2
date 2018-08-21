#ifndef UI_USER_INTERFACE_H
#define UI_USER_INTERFACE_H

#include "noise_gen/module_manager.h"

#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Math/Vector2.h>
#include <MagnumImgui.h>

class UserInterface
{
public:
    UserInterface();
    ~UserInterface();

    void render(ModuleManager& manager);

private:
    std::string selected_module_;
};

#endif // UI_USER_INTERFACE_H