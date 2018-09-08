#ifndef DETAIL_VIEW_PARAMETER_VIEW_VISITOR_H
#define DETAIL_VIER_PARAMETER_VIEW_VISITOR_H

#include "noise_gen/noise_module.h"
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

            bool operator()(NoiseModule::Ref& module) const
            {
                return false;
            }

        private:
            std::string param_name_;
        };
    }
}


#endif //