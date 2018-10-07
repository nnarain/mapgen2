#ifndef DETAIL_VIEW_PARAMETER_VIEW_VISITOR_H
#define DETAIL_VIER_PARAMETER_VIEW_VISITOR_H

#include "noise_gen/noise_module.h"
#include <noiseutils/noiseutils.h>
#include <boost/variant.hpp>
#include <imgui.h>

namespace detail
{
    namespace view
    {
        struct ParameterViewVistor : public boost::static_visitor<bool>
        {
            ParameterViewVistor(const std::string& param_name)
                : param_name_{ param_name }
            {
            }

            bool operator()(int& i) const
            {
                return ImGui::DragInt(param_name_.c_str(), &i, 1);
            }

            bool operator()(float& f) const
            {
                return ImGui::DragFloat(param_name_.c_str(), &f, 0.01f);
            }

            bool operator()(RangedInt& ri) const
            {
                return ImGui::SliderInt(param_name_.c_str(), &ri.value, ri.min, ri.max);
            }

            bool operator()(RangedFloat& rf) const
            {
                return ImGui::SliderFloat(param_name_.c_str(), &rf.value, rf.min, rf.max);
            }

            bool operator()(bool& b) const
            {
                return ImGui::Checkbox(param_name_.c_str(), &b);
            }

            bool operator()(noise::utils::Color& c) const
            {
                const float r = (float)c.red / 255.0f;
                const float g = (float)c.green / 255.0f;
                const float b = (float)c.blue / 255.0f;
                const float a = (float)c.alpha / 255.0f;

                float colors[4] = { r, g, b, a };

                auto updated = ImGui::ColorEdit4(param_name_.c_str(), colors, ImGuiColorEditFlags_NoInputs);

                c.red = (noise::uint8)(colors[0] * 255.0f);
                c.green = (noise::uint8)(colors[1] * 255.0f);
                c.blue = (noise::uint8)(colors[2] * 255.0f);
                c.alpha = (noise::uint8)(colors[3] * 255.0f);

                return updated;
            }

        private:
            std::string param_name_;
        };
    }
}


#endif //