#ifndef GRAPHICS_DATABASE__MODEL_H_
#define GRAPHICS_DATABASE__MODEL_H_
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

class Batch;
class Matrix44;

class Model
{
private:
    Batch* batch_;
    Vector3 position_;
    Vector3 angle_;

public:
    Model();
    Model(Batch* batch);
    ~Model();
    const Vector3* position() const;
    void position(const Vector3& new_value);
    const Vector3* angle() const;
    void angle(const Vector3& new_value);
    void draw(const Matrix44& perspective_matrix);
    const Vector3* vertexes() const;
    size_t vertexes_size() const;
    const int* indexes() const;
    size_t indexes_size() const;
};

} // namespace GraphicsDatabase

#endif
