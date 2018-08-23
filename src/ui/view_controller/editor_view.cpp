#include "ui/view_controller/editor_view.h"

#include <imgui.h>

#include <iostream>

static constexpr const char* MODULE_TYPES[] = {
    "perlin",
    "select"
};

struct ParameterViewVistor : public boost::static_visitor<>
{


    ParameterViewVistor(const std::string& module_name, const std::string& param_name, ModuleManagerController& manager)
        : module_name_{module_name}
        , param_name_{ param_name }
        , manager{ manager }
    {
    }

    ~ParameterViewVistor()
    {
    }

    void operator()(int& i)
    {
        ImGui::DragInt(param_name_.c_str(), &i, 1);
    }

    void operator()(float& f)
    {
        ImGui::DragFloat(param_name_.c_str(), &f, 0.01f);
    }

    void operator()(RangedInt& ri)
    {
        ImGui::SliderInt(param_name_.c_str(), &ri.value, ri.min, ri.max);
    }

    void operator()(RangedFloat& rf)
    {
        ImGui::SliderFloat(param_name_.c_str(), &rf.value, rf.min, rf.max);
    }

    void operator()(NoiseModule*& module)
    {
        const auto& module_names = manager.getModuleNames();
        const char* current_item = (module) ? module->getName().c_str() : nullptr;

        if (ImGui::BeginCombo(param_name_.c_str(), current_item))
        {
            for (const auto& module_name : module_names)
            {
                // TODO: do not allow adding self as parameter
                if (module_name == module_name_) continue;

                if (ImGui::Selectable(module_name.c_str(), false))
                {
                    module = manager.get(module_name).get();
                }
            }

            ImGui::EndCombo();
        }
    }

private:
    std::string module_name_;
    std::string param_name_;
    ModuleManagerController& manager;
};

EditorView::EditorView(ModuleManagerController& manager)
    : manager_{manager}
{

}

void EditorView::render()
{
    const auto& names = manager_.getModuleNames();

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Editor"))
    {
        // module list
        static constexpr const char* module_select_id = "module_select";
        ImGui::BeginChild(module_select_id, ImVec2(150, 0), true);
        {
            for (const auto& name : names)
            {
                if (ImGui::Selectable(name.c_str(), selected_module_ == name))
                {
                    selected_module_ = name;
                }
            }
        }
        ImGui::EndChild();

        // Context menu for the module select pane
        if (ImGui::BeginPopupContextItem(module_select_id))
        {
            static char buf[32] = "module1";

            ImGui::Text("Module name:");
            ImGui::InputText("##edit", buf, IM_ARRAYSIZE(buf));

            ImGui::Text("Module Type:");
            static int type_select = 0;
            ImGui::Combo("", &type_select, MODULE_TYPES, IM_ARRAYSIZE(MODULE_TYPES));

            if (ImGui::Button("add"))
            {
                NoiseModule::Type type = static_cast<NoiseModule::Type>(type_select);
                std::string name(buf);

                if (!manager_.has(name))
                {
                    manager_.createModule(name, type);
                    std::strncpy(buf, "module1", IM_ARRAYSIZE(buf));
                }
                else
                {
                    std::cout << "Module with name, " << name << ", already exists" << std::endl;
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine();

        ImGui::BeginChild("parameter pane");
        {
            if (manager_.has(selected_module_))
            {
                auto& module = manager_.get(selected_module_);

                ImGui::Text("Type: %s", MODULE_TYPES[static_cast<int>(module->getType())]);
                ImGui::Separator();

                auto params = module->getParams();

                for (auto& param_iter : *params)
                {
                    ParameterViewVistor parameter_view{ module->getName(), param_iter.first, manager_ };
                    boost::apply_visitor(parameter_view, param_iter.second);
                }
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }
}