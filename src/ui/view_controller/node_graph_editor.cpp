#include "ui/view_controller/node_graph_editor.h"
#include "ui/view_controller/module_preview.h"
#include "noise_gen/noise_module.h"
#include "detail/view/parameter_view_visitor.h"

#include <addons/imguinodegrapheditor/imguinodegrapheditor.h>

#include <imgui.h>

#include <string>
#include <sstream>
#include <iostream>

static const char* NODE_TYPE_NAMES[] = {
    "abs",
    "add",
    "billow",
    "blend",
    "cache",
    "checkerboard",
    "clamp",
    "const",
    "cylinders",
    "displace",
    "exponent",
    "invert",
    "max",
    "min",
    "multiply",
    "perlin",
    "power",
    "ridgedmulti",
    "rotate",
    "scalebias",
    "scale",
    "select",
    "spheres",
    "translate",
    "turbulence",
    "voronoi",
    "output"
};

static const char* NODE_DESCRIPTIONS[] = {
    // Abs
    "Outputs the absolute value of the of source module value",
    // Add
    "Adds the two source modules together",
    // Billow
    "This noise generates 'billow' noise suitable for clouds and rocks.\n"
    "It is nearly identical to Perlin noise except each octave is modified with an absolute value function.",
    // Blend
    "* Source module 0: Outputs one of the values to blend\n"
    "* Source module 1: Ouputs one of the values to blend\n"
    "* Source module 2: Control module. Determines the weight of the blending operation. Negative values weight towards Source 0, Positives values weight"
    "towards source 1.",
    // Cache
    "Caches the output",
    // Checkerboard
    "Outputs a checkerboard pattern. -1 to 1",
    // Clamp
    "Clamps output to specified values",
    // Const
    "Outputs a constant value",
    // Cylinders
    "Outputs concentric cylinders contered on the origin",
    // Displace
    "Uses 3 source modules to displace an input module. Source 0 in the input, 1-3 are displace noise functions",
    // Exponent
    "Maps value to an exponential curve",
    // Invert
    "Inverts the source module value",
    // Max
    "Outputs the max of two source modules",
    // Min
    "Output the minimun of the two source modules",
    // Multiply
    "Multiplies the two source modules",
    // Perlin
    "Perlin noise is the sum of several coherent noise functions of ever increasing frequencies and decreasing amplitudes.\n"
    "A small change in the input value will produce a small change in the output value, while a large change in the input value with produce a random "
    "change in the output value.",
    // Power
    "Raised the first source module to the power of the second source module",
    // Ridged Multi
    "This noise module, heavily based on the Perlin module, generates ridged-multifractal noise. Ridged-multifractal noise is generated in much of the same "
    "way as Perlin noise, except the output of each octave is modified by an absoluate-value function.\n"
    "Ridged-multifractal",
    // Rotate
    "Rotates the output",
    // Scale Bias
    "Retrieves the output value from the source module, multiples it with a scaling factor, adds a bias to it, then outputs the value.",
    // Scale Point
    "Multiple x,y,z from source with x,y,z scaling factors",
    // Select
    "Noise module that outputs the value selected from the one of two source modules chosen by the output value from a control module.",
    // Spheres
    "Outputs concentric spheres centered on the origin. Increasing the frequency reduces the distances between spheres.",
    // Translate
    "Translate source module values",
    // Turbulence
    "Applies pseudo-random displacement on the input value.\n"
    "Frequency determines how fast the displacement value changes.\n"
    "Power determines the scaling factor of the turbulence.\n"
    "Roughness determines the roughness of the changes in turbulence",
    // Voronoi
    "Voronoi cell is a region containing all the points that are closest to a specific seed point.\n"
    "This module randomly places seed points within each unit cube.\n"
    "* Frequency controls the distance between seed points\n"
    "* Displacement control the range of random values assigned to each cell"
    "* Enable Distance causes points in the cells to increase in value the further away that point if from the nearest seed point",
    // Output
    NULL
};

class NoiseNode : public ImGui::Node
{
protected:
    NoiseNode(const std::string& name, const std::string& inputs, NoiseModule::Ptr& module, const ImVec2& pos)
        : ImGui::Node()
        , inputs{ inputs }
        , ref{module}
        , needs_preview_update{true}
        , preview{ { 25, 25 } }
    {
        this->init(module->getName().c_str(), pos, this->inputs.c_str(), "out", static_cast<int>(module->getType()));
    }

    virtual ~NoiseNode()
    {
    }

    virtual const char* getTooltip() const { return NODE_TYPE_NAMES[getType()]; }
    virtual const char* getInfo() const { return NODE_DESCRIPTIONS[getType()]; }

private:
    static std::string getInputsString(NoiseModule& module)
    {
        std::stringstream ss;

        const auto source_count = module.getSourceModuleCount();
        for (int i = 0; i < source_count; ++i)
        {
            ss << std::string("s") << std::to_string(i);
            if (i < source_count - 1)
            {
                ss << std::string(";");
            }
        }

        return ss.str();
    }

public:
    bool render(float node_width) override
    {
        if (needs_preview_update)
        {
            if (auto ptr = ref.lock())
            {
                preview.update(*ptr);
                needs_preview_update = false;
            }
        }

        auto& size = preview.getSize();
        auto aspect = node_width / size.x();
        auto height = aspect * size.y();
        preview.setSize({ node_width, height });

        preview.render();
        return false;
    }

    void renderActive() override
    {
        if (auto module = ref.lock())
        {
            auto updated = module->updateParameters([](const std::string& name, NoiseModule::ParameterVariant& param) -> bool {
                return boost::apply_visitor(detail::view::ParameterViewVistor{ name }, param);
            });

            if (updated)
            {
                setUpdateRequired();
            }
        }

        ImGui::Node::renderActive();
    }

    void setUpdateRequired()
    {
        this->needs_preview_update = true;
        updateOutputLinks();
    }

    void updateOutputLinks()
    {
        auto& nge = getNodeGraphEditor();
        
        ImVector<ImGui::Node*> out_links;
        nge.getOutputNodesForNodeAndSlot(this, 0, out_links);

        for (auto i = 0; i < out_links.size(); ++i)
        {
            auto out_node = out_links[i];
            if (out_node->getType() != NodeGraphEditorTab::NodeTypes::OUTPUT)
            {
                auto out_noise_node = static_cast<NoiseNode*>(out_node);
                out_noise_node->setUpdateRequired();
            }
        }
    }

    static NoiseNode* Create(NoiseModule::Ptr& module, const ImVec2& pos)
    {
        const auto& name = module->getName();
        const auto inputs = getInputsString(*module);

        NoiseNode* node = (NoiseNode*)ImGui::MemAlloc(sizeof(NoiseNode));
        IM_PLACEMENT_NEW(node) NoiseNode(name, inputs, module, pos);

        node->baseWidthOverride = 125.f;

        return node;
    }

    std::string inputs;
    NoiseModule::Ref ref;
    ModulePreview preview;

private:
    bool needs_preview_update;
};

class OutputNode : public ImGui::Node
{
protected:
    OutputNode() : ImGui::Node()
    {

    }

public:
    static OutputNode* Create(const ImVec2& pos)
    {
        OutputNode* node = (OutputNode*)ImGui::MemAlloc(sizeof(NoiseNode));
        IM_PLACEMENT_NEW(node) OutputNode();

        node->init("output", pos, "in", "", NodeGraphEditorTab::NodeTypes::OUTPUT);
        node->setOpen(false);

        return node;
    }
};

NodeGraphEditorTab::NodeGraphEditorTab(NoiseMapManager& manager)
    : manager_{ manager }
{
}

NodeGraphEditorTab::~NodeGraphEditorTab()
{
}

void NodeGraphEditorTab::renderTab()
{
    if (auto ptr = current_editor_.lock())
    {
        ptr->render();
    }
}

void NodeGraphEditorTab::createNodeGraphEditor(const std::string& name, NoiseMap::Ptr& noisemap)
{
    std::shared_ptr<ImGui::NodeGraphEditor> nge = std::make_shared<ImGui::NodeGraphEditor>();

    nge->registerNodeTypes(NODE_TYPE_NAMES, NodeTypes::NODE_TYPE_COUNT, &NodeGraphEditorTab::nodeFactory);
    nge->user_ptr = &(*noisemap);

    nge->setLinkCallback(&NodeGraphEditorTab::linkCallback);
    nge->setNodeCallback(&NodeGraphEditorTab::nodeCallback);

    nge->registerNodeTypeMaxAllowedInstances(NodeTypes::OUTPUT, 1);

    auto& io = ImGui::GetIO();

    const auto w = io.DisplaySize.x;
    const auto h = io.DisplaySize.y;
    
    nge->addNode(NodeTypes::OUTPUT, { w / 2, h / 2 });

    // style
    nge->show_style_editor = false;
    nge->show_top_pane = false;

    editors_[name] = nge;
}

void NodeGraphEditorTab::removeNodeGraphEditor(const std::string& name)
{
    editors_.erase(name);
}

void NodeGraphEditorTab::selectNodeGraphEditor(const std::string& name)
{
    if (editors_.find(name) != editors_.end())
    {
        current_editor_ = editors_[name];
    }
}

ImGui::Node* NodeGraphEditorTab::nodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& nge)
{
    if (nt != NodeTypes::OUTPUT)
    {
        auto& map = *static_cast<NoiseMap*>(nge.user_ptr);
        auto& module = map.createModuleWithUniqueName(static_cast<NoiseModule::Type>(nt));
        module->update();

        return NoiseNode::Create(module, pos);
    }
    else
    {
        return OutputNode::Create(pos);
    }
}

void NodeGraphEditorTab::linkCallback(const ImGui::NodeLink& link, ImGui::NodeGraphEditor::LinkState state, ImGui::NodeGraphEditor& nge)
{
    // input node is always a noise node
    auto in_node = static_cast<NoiseNode*>(link.InputNode);

    // when a link is added
    if (state == ImGui::NodeGraphEditor::LinkState::LS_ADDED)
    {
        // check if the output node is not a output node type
        if (link.OutputNode->getType() != NodeTypes::OUTPUT)
        {
            // output node is a noise node
            auto out_node = static_cast<NoiseNode*>(link.OutputNode);
            
            // set the input noise in the output node
            if (auto in_noise = in_node->ref.lock())
            {
                if (auto out_noise = out_node->ref.lock())
                {
                    auto out_slot = link.OutputSlot;
                    out_noise->setSourceModule(out_slot, in_noise);
                    out_node->setUpdateRequired();
                }
            }
        }
        else
        {
            // The output node is the output noise node of the editor
            // Set the output module in the map, to notify observer of the output module change
            auto& map = *static_cast<NoiseMap*>(nge.user_ptr);
            map.setOutputModule(in_node->ref);
        }
    }
    else
    {
        // a link was deleted, remove its reference in the output module
        if (link.OutputNode->getType() != NodeTypes::OUTPUT)
        {
            auto out_node = static_cast<NoiseNode*>(link.OutputNode);
            if (auto module = out_node->ref.lock())
            {
                module->setSourceModule(link.InputSlot, {});
            }
        }
        else
        {
            // link was removed from the output node
            auto& map = *static_cast<NoiseMap*>(nge.user_ptr);
            map.setOutputModule(NoiseModule::Ref{});
        }
    }
}

void NodeGraphEditorTab::nodeCallback(ImGui::Node*& node, ImGui::NodeGraphEditor::NodeState state, ImGui::NodeGraphEditor& nge)
{
    auto& map = *static_cast<NoiseMap*>(nge.user_ptr);

    if (state == ImGui::NodeGraphEditor::NodeState::NS_DELETED)
    {
        if (node->getType() != NodeGraphEditorTab::NodeTypes::OUTPUT)
        {
            // get the noise module for this node
            auto* noise_node = static_cast<NoiseNode*>(node);
            if (auto module = noise_node->ref.lock())
            {
                // delete the module in map
                map.remove(module->getName());
            }
        }
    }
    else if (state == ImGui::NodeGraphEditor::NodeState::NS_EDITED)
    {
        if (node->getType() != NodeGraphEditorTab::NodeTypes::OUTPUT)
        {
            auto* noise_node = static_cast<NoiseNode*>(node);

            // compare the name as displayed in the editor the the underlying noise module name
            const std::string editor_name(node->getName());
            
            if (auto module = noise_node->ref.lock())
            {
                auto noise_name = module->getName();

                if (editor_name != noise_name)
                {
                    // The name has changed. Rename the node.
                    map.rename(noise_name, editor_name);
                }
            }
        }
    }
}

void NodeGraphEditorTab::onMapEvent(MapEvent event, std::string map_name)
{
    switch (event)
    {
    case MapEvent::Created:
        createNodeGraphEditor(map_name, manager_.getNoiseMap(map_name));
        break;
    case MapEvent::Removed:
        removeNodeGraphEditor(map_name);
        break;
    case MapEvent::Changed:
        selectNodeGraphEditor(map_name);
        break;
    }
}
