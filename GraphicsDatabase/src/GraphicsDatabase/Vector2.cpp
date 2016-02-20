#include "GraphicsDatabase/Vector2.h"

namespace GraphicsDatabase
{

Vector2::Vector2() : u(0.0), v(0.0) {}

Vector2::Vector2(double new_u, double new_v) : u(new_u), v(new_v) {}

// Vector2::~Vector2();

void Vector2::copy_from(const Vector2& base)
{
    u = base.u;
    v = base.v;
}

void Vector2::set(double new_u, double new_v)
{
    u = new_u;
    v = new_v;
}

} // namespace GraphicsDatabase
