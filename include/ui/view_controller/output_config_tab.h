#ifndef UI_VIEW_CONTROLLER_OUTPUT_DISPLAY_H
#define UI_VIEW_CONTROLLER_OUTPUT_DISPLAY_H

#include "ui/view_controller/tab_renderer.h"
#include "ui/view_controller/module_preview.h"
#include "ui/view_controller/module_manager_controller.h"

class OutputConfigTab : public TabRenderer
{
public:
    OutputConfigTab(ModuleManagerController& manager);
    ~OutputConfigTab();

    virtual void renderTab() override;

    void onOutputChanged(NoiseModule::Ref ref);

private:
    bool renderModuleParameters(NoiseModule& module);

    ModuleManagerController& manager_;
    ModulePreview preview_;

    NoiseModule::Ref output_module_;
    bool update_required_;

    int seed_;
};

#endif