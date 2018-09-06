#ifndef UI_VIEW_CONTROLLER_TAB_RENDERER_H
#define UI_VIEW_CONTROLLER_TAB_RENDERER_H

#include <memory>

class TabRenderer
{
public:
    using Ptr = std::unique_ptr<TabRenderer>;

    virtual void renderTab() = 0;
};

#endif