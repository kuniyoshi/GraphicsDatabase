#include "GraphicsDatabase/Node.h"
#include <cassert>
#include <string>
#include "GraphicsDatabase/Animation.h"
#include "GraphicsDatabase/Matrix44.h"
#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/NodeTemplate.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

Node::Node(const std::string& id)
:   id_(id), model_(0),
    children_(0), children_size_(0),
    does_model_in_database_(true)
{}

Node::~Node()
{
    if (!does_model_in_database_)
    {
        delete model_;
    }
    else
    {
        ; // will be deleted at ~Database
    }

    model_ = 0;

    for (size_t i = 0; i < children_size_; ++i)
    {
        delete children_[i];
        children_[i] = 0;
    }

    if (children_)
    {
        delete[] children_;
        children_ = 0;
    }
}

const std::string* Node::id() const { return &id_; }

Model* Node::model() { return model_; }

const Model* Node::model() const { return model_; }

void Node::model(Model* new_value) { model_ = new_value; }

size_t Node::children_size() const { return children_size_; }

void Node::children_size(int new_value)
{
    assert(children_size_ == 0 && new_value >= 0);
    children_size_ = new_value;
}

void Node::set_child(Node* child, int index)
{
    assert(index >= 0 && static_cast< unsigned >(index) <= children_size_);
    children_[index] = child;
}

Node* Node::get_child(int index)
{
    assert(index >= 0 && static_cast< unsigned >(index) <= children_size_);
    return children_[index];
}

const Node* Node::get_child(int index) const
{
    assert(index >= 0 && static_cast< unsigned >(index) <= children_size_);
    return children_[index];
}

void Node::setup_model(const NodeTemplate& node_template)
{
    if (!model_)
    {
        model_ = new Model;
        does_model_in_database_ = false;
    }

    const double* scale = node_template.scale();
    model_->scale(Vector3(scale[0], scale[1], scale[2]));
    const double* angle = node_template.angle();
    model_->angle(Vector3(angle[0], angle[1], angle[2]));
    const double* position = node_template.position();
    model_->position(Vector3(position[0], position[1], position[2]));
}

void Node::reserve_children()
{
    assert(!children_);
    assert(children_size_ != 0);

    children_ = new Node*[children_size_];

    for (size_t i = 0; i < children_size_; ++i)
    {
        children_[i] = 0;
    }
}

void Node::draw(    const Matrix44& perspective_matrix,
                    const Vector3& brightness,
                    double ambient_brightness,
                    const Vector3& light_vector) const
{
    model_->draw(   perspective_matrix,
                    brightness,
                    ambient_brightness,
                    light_vector);

    Matrix44 wcpm(model_->world_matrix());
    wcpm.dot(perspective_matrix);

    for (size_t i = 0; i < children_size_; ++i)
    {
        children_[i]->draw( wcpm,
                            brightness,
                            ambient_brightness,
                            light_vector);
    }
}

void Node::draw_flat_shading(   const Matrix44& perspective_matrix,
                                const Vector3& brightness,
                                double ambient_brightness,
                                const Vector3& light_vector) const
{
    model_->draw_flat_shading(  perspective_matrix,
                                brightness,
                                ambient_brightness,
                                light_vector);

    Matrix44 wcpm(model_->world_matrix());
    wcpm.dot(perspective_matrix);

    for (size_t i = 0; i < children_size_; ++i)
    {
        children_[i]->draw_flat_shading(    wcpm,
                                            brightness,
                                            ambient_brightness,
                                            light_vector);
    }
}

void Node::update(const double time, const Animation& animation)
{
    Vector3 value;

    if (animation.has_scale_completions())
    {
        animation.scale_at(&value, time);
        model_->scale(value);
    }

    if (animation.has_angle_completions())
    {
        animation.angle_at(&value, time);
        model_->angle(value);
    }

    if (animation.has_position_completions())
    {
        animation.position_at(&value, time);
        model_->position(value);
    }
}

} // namespace GraphicsDatabase
