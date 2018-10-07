#include "ui/view_controller/output_config_tab.h"
#include "detail/view/parameter_view_visitor.h"

#include <imgui.h>

static constexpr float TEXTURE_SIZE = 256.0f;

OutputConfigTab::OutputConfigTab(NoiseMapManager& manager)
    : manager_{manager}
    , update_required_{false}
    , seed_{0}
{
    
}

OutputConfigTab::~OutputConfigTab()
{
}

void OutputConfigTab::renderTab()
{
    constexpr float width_percent = 0.33f;

    if (auto preview = current_preview_.lock())
    {
        // update output if it changed
        if (update_required_)
        {
            update_required_ = false;

            if (auto module = output_module_.lock())
            {
                preview->update(*module);
            }
        }

        auto window_size = ImGui::GetWindowSize();

        ImGui::BeginChild("parameter panel", { window_size.x * width_percent, window_size.y }, true);
        {
            if (ImGui::CollapsingHeader("Renderer Configuration", ImGuiTreeNodeFlags_DefaultOpen))
            {
                preview->renderParameters();
            }

            if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (auto manager = current_noise_map_.lock())
                {
                    if (ImGui::DragInt("seed", &seed_))
                    {
                        manager->setSeed(seed_);
                        update_required_ = true;
                    }

                    manager->forEach([this](const std::string&, NoiseModule& module) {
                        auto updated = renderModuleParameters(module);
                        if (updated)
                        {
                            module.update();
                        }
                        update_required_ = update_required_ || updated;
                    });
                }
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // set the preview image size
        auto cursor_pos = ImGui::GetCursorPos();
        auto preview_width = window_size.x - cursor_pos.x;
        auto preview_height = window_size.y - cursor_pos.y;

        preview->setSize({ preview_width, preview_height - 50 });
        preview->render();
    }
}

bool OutputConfigTab::renderModuleParameters(NoiseModule& module)
{
    const auto& module_name = module.getName();

    auto updated = module.updateParameters([&module_name](const std::string& name, NoiseModule::ParameterVariant& param) -> bool {
        if (name == "seed")
        {
            return false;
        }
        auto full_name = module_name + "." + name;
        return boost::apply_visitor(detail::view::ParameterViewVistor{ full_name }, param);
    });

    return updated;
}

void OutputConfigTab::onOutputChanged(NoiseModule::Ref ref)
{
    output_module_ = ref;
    update_required_ = true;
}

void OutputConfigTab::onMapEvent(MapEvent event, std::string name)
{
    if (event == MapEvent::Created)
    {
        auto& map = manager_.getNoiseMap(name);
        map->connectOutputChanged(std::bind(&OutputConfigTab::onOutputChanged, this, std::placeholders::_1));

        // create a module preview for each noise map
        previews_[name] = ModulePreview::Ptr{ new ModulePreview({ 256, 256 },{ TEXTURE_SIZE, TEXTURE_SIZE }) };
    }
    else if (event == MapEvent::Removed)
    {
        previews_.erase(name);
    }
    else
    {
        auto& map = manager_.getNoiseMap(name);
        current_noise_map_ = map;
        current_preview_ = previews_[name];
    }

}