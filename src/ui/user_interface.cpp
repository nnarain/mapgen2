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

    ParameterViewVistor(const std::string& name) : name{name}
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

    void operator()(noise::module::Module*& module)
    {
        // TODO: Hm
        ImGui::Text("TBD");
    }
};

UserInterface::UserInterface()
{

}

UserInterface::~UserInterface()
{
}

void UserInterface::render(ModuleManager& manager)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Editor"))
    {
        // module list
        static constexpr const char* module_select_id = "module_select";
        ImGui::BeginChild(module_select_id, ImVec2(150, 0), true);
        {
            manager.forEach([this](const std::string& name, NoiseModule& module)
            {
                if (ImGui::Selectable(name.c_str(), selected_module_ == name))
                {
                    selected_module_ = name;
                }
            });
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

                if (!manager.has(name))
                {
                    manager.create(name, type);
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
            if (manager.has(selected_module_))
            {
                auto& module = manager.get(selected_module_);
                auto params = module->getParams();

                for (auto& param_iter : *params)
                {
                    ParameterViewVistor parameter_view{ param_iter.first };
                    boost::apply_visitor(parameter_view, param_iter.second);
                }
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }
}
