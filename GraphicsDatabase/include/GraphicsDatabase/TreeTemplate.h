#ifndef GRAPHICS_DATABASE__TREE_TEMPLATE_H_
#define GRAPHICS_DATABASE__TREE_TEMPLATE_H_
#include <string>

namespace GraphicsDatabase
{

class NodeTemplate;

class TreeTemplate
{
private:
    const std::string id_;
    NodeTemplate* root_;

public:
    TreeTemplate(const std::string& id);
    TreeTemplate(const std::string& id, NodeTemplate* root);
    ~TreeTemplate();
    const std::string* id() const;
    const NodeTemplate* root() const;
    void root(NodeTemplate* new_value);
};

} // namespace GraphicsDatabase

#endif
