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
        , manager_{ manager }
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
        const auto& module_names = manager_.getModuleNames();
        const char* current_item = (module) ? module->getName().c_str() : nullptr;

        if (ImGui::BeginCombo(param_name_.c_str(), current_item))
        {
            for (const auto& module_name : module_names)
            {
                if (module_name == module_name_) continue;

                if (ImGui::Selectable(module_name.c_str(), false))
                {
                    module = manager_.get(module_name).get();
                }
            }

            ImGui::EndCombo();
        }
    }

private:
    std::string module_name_;
    std::string param_name_;
    ModuleManagerController& manager_;
};

struct ModuleCounterVistor : public boost::static_visitor<>
{
public:
    ModuleCounterVistor() : count_{ 0 } {}
    void operator()(int&){}
    void operator()(float&){}
    void operator()(RangedInt&){}
    void operator()(RangedFloat&){}

    void operator()(NoiseModule*& module)
    {
        count_++;
    }

    int getCount() const
    {
        return count_;
    }

private:
    int count_;
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

                // given the selected type, create a new module
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

        // Draw parameters in the right pane
        ImGui::BeginChild("parameter pane");
        {
            if (manager_.has(selected_module_))
            {
                auto& module = manager_.get(selected_module_);

                // draw noise tyoe
                ImGui::Text("Type: %s", MODULE_TYPES[static_cast<int>(module->getType())]);

                // draw parameters
                auto params = module->getParams();

                // count number of source modules are accounted for in parameters
                ModuleCounterVistor count;

                for (auto& param_iter : *params)
                {
                    ParameterViewVistor parameter_view{ module->getName(), param_iter.first, manager_ };
                    boost::apply_visitor(parameter_view, param_iter.second);
                    boost::apply_visitor(count, param_iter.second);
                }

                ImGui::Separator();

                // draw source module selection
                auto& module_base = module->getModule();

                // get the number of source modules
                auto source_count = module_base->GetSourceModuleCount();
                auto actual_source_count = source_count - count.getCount();

                if (actual_source_count > 0)
                    ImGui::Text("Sources");

                for (int i = 0; i < actual_source_count; ++i)
                {
                    // souce module name
                    std::string source_name = "source " + std::to_string(i + 1);
                    // current source module
                    const auto& module_ptr = module_base->GetSourceModule(i);
                    // get the name of the module
                    const auto& current_item_name = manager_.lookupName(module_ptr);

                    // create a combo box to select source modules
                    if (ImGui::BeginCombo(source_name.c_str(), current_item_name.c_str()))
                    {
                        for (const auto& name : names)
                        {
                            // do not allow module to add itself as a source
                            if (name == selected_module_) continue;

                            if (ImGui::Selectable(name.c_str(), name == current_item_name))
                            {
                                module_base->SetSourceModule(i, *manager_.get(name)->getModule().get());
                            }
                        }

                        ImGui::EndCombo();
                    }
                }
            }
        }
        ImGui::EndChild();

    }
    ImGui::End();
}