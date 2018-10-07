#ifndef UI_VIEW_CONTROLLER_MAP_SELECT_TAB_H
#define UI_VIEW_CONTROLLER_MAP_SELECT_TAB_H

#include "ui/view_controller/tab_renderer.h"
#include "ui/events/map_event.h"
#include "noise_gen/noise_map_manager.h"

#include <boost/signals2.hpp>

#include <map>
#include <string>
#include <functional>

/**
    Select the noise map to interact with
*/
class MapSelectTab : public TabRenderer
{
public:
    using Ptr = std::unique_ptr<MapSelectTab>;

    MapSelectTab(NoiseMapManager& manager);
    ~MapSelectTab() = default;

    virtual void renderTab() override;

    void connect(std::function<void(MapEvent, std::string)>);
private:
    NoiseMapManager& manager_;

    boost::signals2::signal<void(MapEvent, std::string)> on_map_event_;
    std::string selected_;
};


#endif //