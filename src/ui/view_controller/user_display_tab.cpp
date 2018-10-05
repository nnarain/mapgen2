#include "ui/view_controller/user_display_tab.h"
#include "detail/view/parameter_view_visitor.h"

#include <Magnum/ImageView.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/TextureFormat.h>
#include <Corrade/Containers/ArrayView.h>

#include <imgui.h>
#include <MagnumImGui.h>

#include <iostream>


UserDisplayTab::UserDisplayTab(NoiseMapManager& manager)
    : manager_{manager}
    , target_{}
    , target_size_{{256, 256}}
{
    target_
        .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear);
}

void UserDisplayTab::renderTab()
{
    constexpr float width_percent = 0.25f;

    auto window_size = ImGui::GetWindowSize();

    ImGui::BeginChild("parameter panel", { window_size.x * width_percent, window_size.y }, true);
    {
        if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto& param : user_params_)
            {
                auto updated = boost::apply_visitor(detail::view::ParameterViewVistor{ param.first }, param.second);

                if (updated)
                {
                    // update the generator
                    if (auto generator = generator_.lock())
                    {
                        try
                        {
                            // update parameters
                            generator->update(user_params_, manager_);
                            // generate output texture
                            generator->generate(target_, target_size_);
                        }
                        catch (std::runtime_error& e)
                        {
                            std::cout << "Exception thrown generating user map\n" << e.what();
                        }
                    }
                }
            }
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    //
    auto cursor_pos = ImGui::GetCursorPos();
    auto width = window_size.x - cursor_pos.x;
    auto height = window_size.y - cursor_pos.y;

    ImGui::Image(target_, { width, height });
}

void UserDisplayTab::setUserGenerator(std::weak_ptr<PluginBase> generator)
{
    // set the current generator
    generator_ = generator;

    if (auto plugin = generator.lock())
    {
        // initialize parameters from the generator
        user_params_ = plugin->initializeParameters();
    }
}

void UserDisplayTab::onMapEvent(MapEvent event, std::string name)
{
    if (event == MapEvent::Created)
    {
        
    }
    else if (event == MapEvent::Removed)
    {

    }
}