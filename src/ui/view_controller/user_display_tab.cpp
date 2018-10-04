#include "ui/view_controller/user_display_tab.h"
#include "detail/view/parameter_view_visitor.h"

#include <Magnum/ImageView.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/TextureFormat.h>
#include <Corrade/Containers/ArrayView.h>

#include <imgui.h>
#include <MagnumImGui.h>


UserDisplayTab::UserDisplayTab()
    : target_{}
    , target_size_{{256, 256}}
    , surface_{target_size_.x(), target_size_.y()}
{

}

void UserDisplayTab::renderTab()
{
    constexpr float width_percent = 0.33f;

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
                        generator->generate(surface_);
                        copySurfaceToTexture(surface_, target_);
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

void UserDisplayTab::copySurfaceToTexture(const Surface& surface, Magnum::GL::Texture2D& target)
{
    // get the surface's color buffer
    const auto& buffer = surface.getBuffer();
    // create an array view for this data
    Corrade::Containers::ArrayView<char> data_view{ (char*)&buffer[0], buffer.size() };
    // create an image2d view to bind to the texture
    Magnum::ImageView2D image{ Magnum::PixelFormat::RGB8Unorm, target_size_, data_view };
    // bind image to texture
    target
        .setWrapping(Magnum::GL::SamplerWrapping::ClampToEdge)
        .setMagnificationFilter(Magnum::GL::SamplerFilter::Linear)
        .setMinificationFilter(Magnum::GL::SamplerFilter::Linear)
        .setStorage(1, Magnum::GL::TextureFormat::RGB8, image.size())
        .setSubImage(0, {}, image);
}