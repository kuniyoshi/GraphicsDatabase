#include "GraphicsDatabase/NodeTemplate.h"
#include <cassert>
#include <vector>

namespace GraphicsDatabase
{

NodeTemplate::NodeTemplate(const std::string& id)
:   id_(id),
    model_id_(),
    children_(0), children_size_(0),
    scale_(1.0)
{
    position_[0] = 0.0;
    position_[1] = 0.0;
    position_[2] = 0.0;
    angle_[0] = 0.0;
    angle_[1] = 0.0;
    angle_[2] = 0.0;
}

NodeTemplate::~NodeTemplate()
{
    for (size_t i = 0; i < children_size_; ++i)
    {
        delete children_[i];
        children_[i] = 0;
    }

    if (!!children_)
    {
        delete[] children_;
        children_ = 0;
    }
}

const std::string* NodeTemplate::id() const { return &id_; }

const std::string* NodeTemplate::model_id() const { return &model_id_; }

void NodeTemplate::model_id(const std::string& new_value)
{
    model_id_ = new_value;
}

size_t NodeTemplate::children_size() const { return children_size_; }

void NodeTemplate::children_size(int new_value)
{
    assert(!children_);
    assert(!children_size_ && new_value >= 0);
    children_size_ = new_value;
}

double NodeTemplate::scale() const { return scale_; }

void NodeTemplate::scale(double new_value) { scale_ = new_value; }

const double* NodeTemplate::angle() const { return angle_; }

void NodeTemplate::angle(const std::vector< double >& angle)
{
    angle_[0] = angle.at(0);
    angle_[1] = angle.at(1);
    angle_[2] = angle.at(2);
}

const double* NodeTemplate::position() const { return position_; }

void NodeTemplate::position(const std::vector< double >& position)
{
    position_[0] = position.at(0);
    position_[1] = position.at(1);
    position_[2] = position.at(2);
}

const NodeTemplate* NodeTemplate::child(int index) const
{
    assert(index >= 0 && static_cast< unsigned >(index) <= children_size_);
    return children_[index];
}

void NodeTemplate::reserve_children()
{
    children_ = new NodeTemplate*[children_size_];

    for (size_t i = 0; i < children_size_; ++i)
    {
        children_[i] = 0;
    }
}

void NodeTemplate::set_child(NodeTemplate* child, int index)
{
    assert(!!children_ && children_size_);
    assert(index >= 0 && static_cast< unsigned >(index) <= children_size_);
    assert(!children_[index]);
    children_[index] = child;
}

} // namespace GraphicsDatabase
