#include "GraphicsDatabase/TreeTemplate.h"
#include <cassert>
#include <string>
#include "GraphicsDatabase/NodeTemplate.h"

namespace GraphicsDatabase
{

TreeTemplate::TreeTemplate(const std::string& id)
:   root_(0), id_(id)
{}

TreeTemplate::TreeTemplate(const std::string& id, NodeTemplate* root)
:   root_(root), id_(id)
{}

TreeTemplate::~TreeTemplate()
{
    delete root_;
    root_ = 0;
}

const std::string* TreeTemplate::id() const { return &id_; }

const NodeTemplate* TreeTemplate::root() const { return root_; }

void TreeTemplate::root(NodeTemplate* new_value)
{
    assert(!root_);
    root_ = new_value;
}

} // namespace GraphicsDatabase
