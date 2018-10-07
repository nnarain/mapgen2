#ifndef UI_VIEW_CONTROLLER_USER_DISPLAY_H
#define UI_VIEW_CONTROLLER_USER_DISPLAY_H

#include "ui/view_controller/tab_renderer.h"
#include "noise_gen/noise_map_manager.h"
#include "plugin/plugin_base.h"
#include "ui/events/map_event.h"

#include <Magnum/GL/Texture.h>

#include <map>
#include <string>
#include <memory>

/**
    Display output from user generators
*/
class UserDisplayTab : public TabRenderer
{
public:
    using Ptr = std::unique_ptr<UserDisplayTab>;

    UserDisplayTab(NoiseMapManager& manager);
    virtual ~UserDisplayTab() = default;

    void renderTab() override;

    void setUserGenerator(std::weak_ptr<PluginBase> generator);

    void onMapEvent(MapEvent event, std::string name);

private:
    // noise map manager
    NoiseMapManager& manager_;
    // active user plugin
    std::weak_ptr<PluginBase> generator_;
    // user parameters
    std::map<std::string, PluginBase::Parameter> user_params_;
    // target texture for user generator
    Magnum::GL::Texture2D target_;
    // target texture size
    Magnum::Vector2i target_size_;
};

#endif