#ifndef UI_VIEW_CONTROLLER_OUTPUT_DISPLAY_H
#define UI_VIEW_CONTROLLER_OUTPUT_DISPLAY_H

#include "ui/view_controller/tab_renderer.h"
#include "ui/view_controller/module_preview.h"
#include "ui/view_controller/module_manager_controller.h"

class OutputDisplayTab : public TabRenderer
{
public:
    OutputDisplayTab(ModuleManagerController& manager);
    ~OutputDisplayTab();

    virtual void renderTab() override;

    void onOutputChanged(NoiseModule::Ref ref);

private:
    ModuleManagerController& manager_;
    ModulePreview preview_;

    NoiseModule::Ref output_module_;
    bool update_required_;
};

#endif