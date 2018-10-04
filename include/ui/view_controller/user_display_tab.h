#ifndef UI_VIEW_CONTROLLER_USER_DISPLAY_H
#define UI_VIEW_CONTROLLER_USER_DISPLAY_H

#include "ui/view_controller/tab_renderer.h"

#include "plugin/plugin_base.h"

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

    UserDisplayTab();
    virtual ~UserDisplayTab() = default;

    void renderTab() override;

    void setUserGenerator(std::weak_ptr<PluginBase> generator);

private:
    /**
        Copy surface to texture
    */
    void copySurfaceToTexture(const Surface& surface, Magnum::GL::Texture2D& target);

    // active user plugin
    std::weak_ptr<PluginBase> generator_;
    // user parameters
    std::map<std::string, PluginBase::Parameter> user_params_;
    // target texture for user generator
    Magnum::GL::Texture2D target_;
    // target texture size
    Magnum::Vector2i target_size_;
    // Drawing surface
    Surface surface_;
};

#endif