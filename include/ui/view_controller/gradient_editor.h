#ifndef UI_VIEW_CONTROLLER_GRADIENT_EDITOR_H
#define UI_VIEW_CONTROLLER_GRADIENT_EDITOR_H

#include <imgui.h>
#include <imgui_color_gradient.h>

#include <vector>

/**
    Manages a single gradient and converts to libnoise colors
*/
class GradientEditor
{
public:
    GradientEditor();

    bool render();

    std::list<ImGradientMark*>& getColorMarks();
private:
    ImGradientMark *dragging_mark_;
    ImGradientMark *selected_mark_;

    ImGradient gradient_;
};

#endif //