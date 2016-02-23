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

public:
    Model();
    Model(Batch* batch);
    ~Model();
    const Vector3& position() const;
    void position(const Vector3& new_value);
    void draw(const Matrix44& perspective_matrix);
};

} // namespace GraphicsDatabase

#endif
