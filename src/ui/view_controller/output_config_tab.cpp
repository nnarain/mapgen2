#include "ui/view_controller/output_config_tab.h"
#include "detail/view/parameter_view_visitor.h"

#include <imgui.h>

static constexpr float TEXTURE_SIZE = 256.0f;

OutputConfigTab::OutputConfigTab(ModuleManagerController& manager)
    : manager_{manager}
    , preview_{ {256, 256}, { TEXTURE_SIZE, TEXTURE_SIZE } }
    , update_required_{false}
    , seed_{0}
{
    manager_.addOutputChangedObserver(std::bind(&OutputConfigTab::onOutputChanged, this, std::placeholders::_1));
}

OutputConfigTab::~OutputConfigTab()
{
}

void OutputConfigTab::renderTab()
{
    constexpr float width_percent = 0.33f;

    // update output if it changed
    if (update_required_)
    {
        update_required_ = false;
        
        if (auto module = output_module_.lock())
        {
            preview_.update(*module);
        }
    }

    auto window_size = ImGui::GetWindowSize();

    ImGui::BeginChild("parameter panel", { window_size.x * width_percent, window_size.y }, true);
    {
        if (ImGui::CollapsingHeader("Renderer Configuration", ImGuiTreeNodeFlags_DefaultOpen))
        {
            preview_.renderParameters();
        }

        if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragInt("seed", &seed_))
            {
                manager_.setSeed(seed_);
                update_required_ = true;
            }

            manager_.forEach([this](const std::string&, NoiseModule& module) {
                auto updated = renderModuleParameters(module);
                if (updated)
                {
                    module.update();
                }
                update_required_ = update_required_ || updated;
            });
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // set the preview image size
    auto cursor_pos = ImGui::GetCursorPos();
    auto preview_width = window_size.x - cursor_pos.x;
    auto preview_height = window_size.y - cursor_pos.y;
    
    preview_.setSize({ preview_width, preview_height - 50 });
    preview_.render();
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