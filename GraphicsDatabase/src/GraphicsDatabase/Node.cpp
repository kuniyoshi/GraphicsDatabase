#include "GraphicsDatabase/Node.h"
#include <cassert>
#include "GraphicsDatabase/Model.h"

namespace GraphicsDatabase
{

Node::Node()
:   model_(0), children_(0), children_size_(0)
{}

Node::~Node()
{
    model_ = 0;
    delete[] children_;
    children_ = 0;
}

void Node::set_model(const Model* model)
{
    model_ = model;
}

void Node::set_child(const Node* child, int index)
{
    assert(index >= 0 && index <= children_size_);
    children_[index] = child;
}

} // namespace GraphicsDatabase
