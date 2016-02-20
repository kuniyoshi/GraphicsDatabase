#ifndef GRAPHICS_DATABASE__VECTOR2_H_
#define GRAPHICS_DATABASE__VECTOR2_H_

namespace GraphicsDatabase
{

class Vector2
{
public:
    double u, v;

public:
    Vector2();
    Vector2(double new_u, double new_v);
    void copy_from(const Vector2& base);
    void set(double new_u, double new_v);
    // ~Vector2();
};

} // namespace GraphicsDatabase

#endif
