#ifndef UI_USER_INTERFACE_H
#define UI_USER_INTERFACE_H

#include "ui/view_controller/view.h"
#include "ui/view_controller/tab_renderer.h"

#include <addons/imguitabwindow/imguitabwindow.h>

#include <vector>
#include <string>

class UserInterface
{
public:
    UserInterface();
    ~UserInterface();

    void render();
    void initialize();

    template<class T, typename... Args>
    void addView(const std::string& name, bool visible, Args&... args)
    {
        View::Ptr view{ new T(args...) };
        view->setVisible(visible);

        views_.push_back({ name, std::move(view) });
    }

    template<class T, typename...Args>
    void addTab(const std::string& name, Args&... args)
    {
        TabRenderer::Ptr tab{ new T(args...) };
        
        tabs_.push_back({ name, std::move(tab) });
    }

private:
    void renderMenu();
    void renderTabs();

    static void tabContentProvider(ImGui::TabWindow::TabLabel* tab, ImGui::TabWindow& parent, void* user_ptr);

    std::vector<std::pair<std::string, View::Ptr>> views_;
    std::vector<std::pair<std::string, TabRenderer::Ptr>> tabs_;
};

#endif // UI_USER_INTERFACE_H