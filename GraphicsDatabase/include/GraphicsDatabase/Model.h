#ifndef GRAPHICS_DATABASE__MODEL_H_
#define GRAPHICS_DATABASE__MODEL_H_
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

class Batch;
class Matrix44;
class Vector3;

class Model
{
private:
    Batch* batch_;
    Vector3 position_;
    Vector3 angle_;
    double scale_;

public:
    Model();
    Model(Batch* batch);
    ~Model();
    const Vector3* position() const;
    void position(const Vector3& new_value);
    const Vector3* angle() const;
    void angle(const Vector3& new_value);
    double scale() const;
    void scale(double new_value);
    void draw(  const Matrix44& perspective_matrix,
                const Vector3& brightness,
                double ambient_brightness,
                const Vector3& light_vector) const;
    void draw_flat_shading( const Matrix44& perspective_matrix,
                            const Vector3& brightness,
                            double ambient_brightness,
                            const Vector3& light_vector) const;
    const Vector3* vertexes() const;
    size_t vertexes_size() const;
    const int* indexes() const;
    size_t indexes_size() const;
    Matrix44 world_matrix() const;
};

} // namespace GraphicsDatabase

#endif
