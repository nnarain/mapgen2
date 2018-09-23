#ifndef UI_VIEW_CONTROLLER_MAP_SELECT_TAB_H
#define UI_VIEW_CONTROLLER_MAP_SELECT_TAB_H

#include "ui/view_controller/tab_renderer.h"

#include <map>
#include <string>

/**
    Select the noise map to interact with
*/
class MapSelectTab : public TabRenderer
{
public:
    MapSelectTab();
    ~MapSelectTab() = default;

    virtual void renderTab() override;
private:
    
    std::string selected_;
};


#endif //