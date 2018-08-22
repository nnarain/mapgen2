#include "ui/user_interface.h"

#include <imgui.h>

#include <iostream>

static constexpr const char* MODULE_TYPES[] = {
    "perlin",
    "select"
};

struct ParameterViewVistor : public boost::static_visitor<>
{
    std::string name;
    ModuleManagerController& manager;

    ParameterViewVistor(const std::string& name, ModuleManagerController& manager) 
        : name{name}
        , manager{manager}
    {
    }

    ~ParameterViewVistor()
    {
    }

    void operator()(int& i)
    {
        ImGui::DragInt(name.c_str(), &i, 1);
    }

    void operator()(float& f)
    {
        ImGui::DragFloat(name.c_str(), &f, 0.01f);
    }

    void operator()(RangedInt& ri)
    {
        ImGui::SliderInt(name.c_str(), &ri.value, ri.min, ri.max);
    }

    void operator()(RangedFloat& rf)
    {
        ImGui::SliderFloat(name.c_str(), &rf.value, rf.min, rf.max);
    }

    void operator()(NoiseModule*& module)
    {
        const auto& module_names = manager.getModuleNames();
        const char* current_item = (module) ? module->getName().c_str() : nullptr;

        if (ImGui::BeginCombo(name.c_str(), current_item))
        {
            for (const auto& module_name : module_names)
            {
                if (ImGui::Selectable(module_name.c_str(), false))
                {
                    module = manager.get(module_name).get();
                }
            }

            ImGui::EndCombo();
        }
    }
};

UserInterface::UserInterface(ModuleManager& manager)
    : manager_{manager}
{

}

UserInterface::~UserInterface()
{
}

void UserInterface::render()
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
                auto params = module->getParams();

                for (auto& param_iter : *params)
                {
                    ParameterViewVistor parameter_view{ param_iter.first, manager_ };
                    boost::apply_visitor(parameter_view, param_iter.second);
                }
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }
}
