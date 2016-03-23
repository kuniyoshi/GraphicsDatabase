#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/Batch.h"
#include "GraphicsDatabase/Matrix44.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

Model::Model()
:   batch_(0),
    position_(),
    angle_(),
    scale_(1.0)
{}

Model::Model(Batch* batch)
:   batch_(batch),
    position_(),
    angle_(),
    scale_(1.0)
{}

Model::~Model() {}

const Vector3* Model::position() const { return &position_; }

void Model::position(const Vector3& new_value)
{
    position_.copy_from(new_value);
}

const Vector3* Model::angle() const { return &angle_; }

void Model::angle(const Vector3& new_value) { angle_.copy_from(new_value); }

double Model::scale() const { return scale_; }

void Model::scale(double new_value) { scale_ = new_value; }

namespace
{

void make_world_matrix( Matrix44* world_matrix,
                        const double scale,
                        const Vector3& angle,
                        const Vector3& position)
{
    world_matrix->scale(scale);
    world_matrix->rotate(angle);
    world_matrix->translate(position);
}

} // namespace -

void Model::draw(   const Matrix44& perspective_matrix,
                    const Vector3& brightness,
                    double ambient_brightness,
                    const Vector3& light_vector) const
{
    if (!batch_)
    {
        return;
    }

    Matrix44 world_matrix;
    make_world_matrix(&world_matrix, scale_, angle_, position_);
    batch_->draw(   world_matrix,
                    perspective_matrix,
                    brightness,
                    ambient_brightness,
                    light_vector);
}

void Model::draw_flat_shading(  const Matrix44& perspective_matrix,
                                const Vector3& brightness,
                                double ambient_brightness,
                                const Vector3& light_vector) const
{
    if (!batch_)
    {
        return;
    }

    Matrix44 world_matrix;
    make_world_matrix(&world_matrix, scale_, angle_, position_);
    batch_->draw_flat_shading(  world_matrix,
                                perspective_matrix,
                                brightness,
                                ambient_brightness,
                                light_vector);
}

const Vector3* Model::vertexes() const { return batch_->vertexes(); }

size_t Model::vertexes_size() const { return batch_->vertexes_size(); }

const int* Model::indexes() const { return batch_->indexes(); }

size_t Model::indexes_size() const { return batch_->indexes_size(); }

Matrix44 Model::translation_matrix() const
{
    Matrix44 translation_matrix;
    translation_matrix.translate(position_);
    return translation_matrix;
}

Matrix44 Model::world_matrix() const
{
    Matrix44 world_matrix;
    make_world_matrix(&world_matrix, scale_, angle_, position_);
    return world_matrix;
}

} // namespace GraphicsDatabase
