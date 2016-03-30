#ifndef GRAPHICS_DATABASE__MATRIX44_H_
#define GRAPHICS_DATABASE__MATRIX44_H_

namespace GraphicsDatabase
{

class Vector3;

class Matrix44
{
private:
    double a0_, a1_, a2_, a3_;
    double b0_, b1_, b2_, b3_;
    double c0_, c1_, c2_, c3_;
    double d0_, d1_, d2_, d3_;

public:
    Matrix44();
    Matrix44(const Matrix44& base);
    ~Matrix44();
    void dot(const Matrix44& operand);
    void multiply(Vector3* operand) const;
    void perspective(   double theta,
                        int width,
                        int heigt,
                        double near_clip,
                        double far_clip);
    void rotate(const Vector3& angle);
    void rotate_yz(double angle_yz);
    void rotate_zx(double angle_zx);
    void rotate_xy(double angle_xy);
    void scale(double a);
    void scale(const Vector3& scale);
    void translate(const Vector3& delta);
};

} // namespace GraphicsDatabase

#endif
