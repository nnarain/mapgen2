#include "ui/view_controller/output_config_tab.h"
#include "detail/view/parameter_view_visitor.h"

#include <imgui.h>

static constexpr float TEXTURE_SIZE = 256.0f;

OutputConfigTab::OutputConfigTab(ModuleManagerController& manager, OutputConfig& config)
    : manager_{manager}
    , config_{config}
    , preview_{ {256, 256}, { TEXTURE_SIZE, TEXTURE_SIZE } }
    , update_required_{false}
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
            update_required_ = preview_.renderParameters();
        }

        if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
        {
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
    auto params = module.getParams();

    auto update_required = false;

    // display checkboxs for items to be exported
    for (auto& param : *params)
    {
        // full parameter name
        const auto& param_name = param.first;
        const auto full_name = module_name + "." + param_name;

        detail::view::ParameterViewVistor v{ full_name };
        auto updated = boost::apply_visitor(v, param.second);

        update_required = update_required || updated;
    }

    return update_required;
}

void OutputConfigTab::onOutputChanged(NoiseModule::Ref ref)
{
    output_module_ = ref;
    update_required_ = true;
}