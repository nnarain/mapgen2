#ifndef UI_USER_INTERFACE_H
#define UI_USER_INTERFACE_H

#include "noise_gen/module_manager.h"
#include "ui/view_controller/module_manager_controller.h"
#include "ui/view_controller/editor_view.h"

class UserInterface
{
public:
    UserInterface(ModuleManager& manager);
    ~UserInterface();

    void render();

private:
    ModuleManagerController manager_;
    // TODO: Remove direct implementation
    EditorView editor_;
};

#endif // UI_USER_INTERFACE_H