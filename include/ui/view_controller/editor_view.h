#ifndef UI_VIEW_CONTROLLER_EDITOR_VIEW_H
#define UI_VIEW_CONTROLLER_EDITOR_VIEW_H

#include "ui/view_controller/view.h"
#include "ui/view_controller/module_manager_controller.h"
#include "ui/view_controller/module_preview.h"

#include <string>

class EditorView : public View
{
public:
    EditorView(ModuleManagerController& manager);

    void render() override;

private:
    ModuleManagerController& manager_;
    ModulePreview preview_;
    std::string selected_module_;
};

#endif // UI_VIEW_CONTROLLER_EDITOR_VIEW_H