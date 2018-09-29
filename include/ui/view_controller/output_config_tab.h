#ifndef UI_VIEW_CONTROLLER_OUTPUT_DISPLAY_H
#define UI_VIEW_CONTROLLER_OUTPUT_DISPLAY_H

#include "ui/view_controller/tab_renderer.h"
#include "ui/view_controller/module_preview.h"
#include "noise_gen/noise_map_manager.h"
#include "ui/events/map_event.h"

class OutputConfigTab : public TabRenderer
{
public:
    using Ptr = std::unique_ptr<OutputConfigTab>;

    OutputConfigTab(NoiseMapManager& manager);
    ~OutputConfigTab();

    virtual void renderTab() override;

    void onMapEvent(MapEvent event, std::string name);
    void onOutputChanged(NoiseModule::Ref ref);

private:
    bool renderModuleParameters(NoiseModule& module);

    NoiseMapManager& manager_;


    NoiseMap::Ref current_noise_map_;
    NoiseModule::Ref output_module_;
    
    ModulePreview preview_;
    bool update_required_;

    int seed_;
};

#endif