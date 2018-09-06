#ifndef UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H
#define UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H

#include "ui/view_controller/tab_renderer.h"

class NodeGraphEditorTab : public TabRenderer
{
public:
    NodeGraphEditorTab();
    virtual ~NodeGraphEditorTab();

    virtual void renderTab() override;
};

#endif // UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H