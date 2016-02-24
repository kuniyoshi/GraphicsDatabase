#ifndef GRAPHICS_DATABASE__VECTOR3_H_
#define GRAPHICS_DATABASE__VECTOR3_H_

namespace GraphicsDatabase
{

class Vector3
{
public:
    double x, y, z, w;

public:
    Vector3();
    Vector3(double new_x, double new_y, double new_z = 0.0, double new_w = 1.0);
    Vector3(const Vector3& base);
    ~Vector3();
    double& operator[](int index);
    Vector3 operator-() const;
    void add(const Vector3& operand);
    void be_reciprocal();
    void clear();
    void copy_from(const Vector3& base);
    double dot(const Vector3& operand);
    void divide(double a);
    void hadamard_product(const Vector3& operand);
    double length() const;
    void multiply(double a);
    void normalize(double max);
    void power(double n);
    void scale(double a);
    void set(double new_x, double new_y, double new_z, double new_w = 1.0);
    void set_xy(double new_x, double new_y);
    double squared_length() const;
    void subtract(const Vector3& operand);
};

} // namespace GraphicsDatabase

#endif
