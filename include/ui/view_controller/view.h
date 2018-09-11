#ifndef UI_VIEW_CONTROLLER_VIEW_H
#define UI_VIEW_CONTROLLER_VIEW_H

#include <memory>

class View
{
public:
    using Ptr = std::unique_ptr<View>;

    View();

    void draw();

    bool isVisible() const noexcept;
    void setVisible(bool visible) noexcept;

protected:
    virtual void render() = 0;

private:
    bool visible_;
};

#endif // UI_VIEW_CONTROLLER_VIEW_H
