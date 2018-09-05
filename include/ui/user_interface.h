#ifndef UI_USER_INTERFACE_H
#define UI_USER_INTERFACE_H

#include "ui/view_controller/view.h"

#include <vector>
#include <string>

class UserInterface
{
public:
    UserInterface();
    ~UserInterface();

    void render();

    template<class T, typename... Args>
    void addView(const std::string& name, bool visible, Args&... args)
    {
        View::Ptr view{ new T(args...) };
        view->setVisible(visible);

        views_.push_back({ name, std::move(view) });
    }

private:
    void renderMenu();

    std::vector<std::pair<std::string, View::Ptr>> views_;
};

#endif // UI_USER_INTERFACE_H