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

void Node::setup_model(const NodeTemplate& node_template)
{
    if (!model_)
    {
        model_ = new Model;
        does_model_in_database_ = false;
    }

    model_->scale(node_template.scale());
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

void Node::update(double time, const Animation& animation)
{
    if (animation.has_scale_completion())
    {
        model_->scale(animation.scale_at(time));
    }

    if (animation.has_angle_completion())
    {
        model_->angle(animation.angle_at(time));
    }

    if (animation.has_position_completion())
    {
        model_->position(animation.position_at(time));
    }
}

} // namespace GraphicsDatabase
