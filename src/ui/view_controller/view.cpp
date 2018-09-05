#include "ui/view_controller/view.h"

View::View()
    : visible_{false}
{

}

void View::draw()
{
    if (!visible_) return;
    render();
}

bool View::isVisible() const noexcept
{
    return visible_;
}

void View::setVisible(bool visible) noexcept
{
    visible_ = visible;
}
