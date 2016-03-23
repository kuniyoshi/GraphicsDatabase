#include "GraphicsDatabase/Node.h"
#include <cassert>
#include "GraphicsDatabase/Matrix44.h"
#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

Node::Node()
:   model_(0), children_(0), children_size_(0)
{}

Node::~Node()
{
    model_ = 0;

    if (children_)
    {
        delete[] children_;
        children_ = 0;
    }
}

void Node::init_children(int children_size)
{
    assert(children_size_ == 0);
    assert(children_ == 0);

    children_size_ = children_size;
    children_ = new Node*[children_size_];
}

void Node::set_model(const Model* model)
{
    assert(!model_);
    model_ = model;
}

void Node::set_child(Node* child, int index)
{
    assert(index >= 0 && index <= children_size_);
    children_[index] = child;
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

    for (int i = 0; i < children_size_; ++i)
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

    for (int i = 0; i < children_size_; ++i)
    {
        children_[i]->draw_flat_shading(    wcpm,
                                            brightness,
                                            ambient_brightness,
                                            light_vector);
    }
}

} // namespace GraphicsDatabase
