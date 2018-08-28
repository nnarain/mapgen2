#include "ui/user_interface.h"

#include <imgui.h>

UserInterface::UserInterface(ModuleManager& manager)
    : manager_{manager}
    , editor_{manager_}
{

}

UserInterface::~UserInterface()
{
}

void UserInterface::render()
{
    editor_.render();
}
