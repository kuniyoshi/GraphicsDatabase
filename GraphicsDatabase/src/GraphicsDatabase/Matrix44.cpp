#include "GraphicsDatabase/Matrix44.h"
#include "GameLib/Math.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

Matrix44::Matrix44()
:   a0_(1.0), a1_(0.0), a2_(0.0), a3_(0.0),
    b0_(0.0), b1_(1.0), b2_(0.0), b3_(0.0),
    c0_(0.0), c1_(0.0), c2_(1.0), c3_(0.0),
    d0_(0.0), d1_(0.0), d2_(0.0), d3_(1.0)
{}

Matrix44::Matrix44(const Matrix44& base)
:   a0_(base.a0_), a1_(base.a1_), a2_(base.a2_), a3_(base.a3_),
    b0_(base.b0_), b1_(base.b1_), b2_(base.b2_), b3_(base.b3_),
    c0_(base.c0_), c1_(base.c1_), c2_(base.c2_), c3_(base.c3_),
    d0_(base.d0_), d1_(base.d1_), d2_(base.d2_), d3_(base.d3_)
{}

Matrix44::~Matrix44() {}

namespace
{

double a_dot(   double a, double b, double c, double d,
                double i, double j, double k, double l)
{
    return a * i + b * j + c * k + d * l;
}

} // namespace -

void Matrix44::dot(const Matrix44& o)
{
    double ta, tb, tc, td;

    ta = a0_;
    tb = b0_;
    tc = c0_;
    td = d0_;
    a0_ = a_dot(o.a0_, o.a1_, o.a2_, o.a3_, ta, tb, tc, td);
    b0_ = a_dot(o.b0_, o.b1_, o.b2_, o.b3_, ta, tb, tc, td);
    c0_ = a_dot(o.c0_, o.c1_, o.c2_, o.c3_, ta, tb, tc, td);
    d0_ = a_dot(o.d0_, o.d1_, o.d2_, o.d3_, ta, tb, tc, td);

    ta = a1_;
    tb = b1_;
    tc = c1_;
    td = d1_;
    a1_ = a_dot(o.a0_, o.a1_, o.a2_, o.a3_, ta, tb, tc, td);
    b1_ = a_dot(o.b0_, o.b1_, o.b2_, o.b3_, ta, tb, tc, td);
    c1_ = a_dot(o.c0_, o.c1_, o.c2_, o.c3_, ta, tb, tc, td);
    d1_ = a_dot(o.d0_, o.d1_, o.d2_, o.d3_, ta, tb, tc, td);

    ta = a2_;
    tb = b2_;
    tc = c2_;
    td = d2_;
    a2_ = a_dot(o.a0_, o.a1_, o.a2_, o.a3_, ta, tb, tc, td);
    b2_ = a_dot(o.b0_, o.b1_, o.b2_, o.b3_, ta, tb, tc, td);
    c2_ = a_dot(o.c0_, o.c1_, o.c2_, o.c3_, ta, tb, tc, td);
    d2_ = a_dot(o.d0_, o.d1_, o.d2_, o.d3_, ta, tb, tc, td);

    ta = a3_;
    tb = b3_;
    tc = c3_;
    td = d3_;
    a3_ = a_dot(o.a0_, o.a1_, o.a2_, o.a3_, ta, tb, tc, td);
    b3_ = a_dot(o.b0_, o.b1_, o.b2_, o.b3_, ta, tb, tc, td);
    c3_ = a_dot(o.c0_, o.c1_, o.c2_, o.c3_, ta, tb, tc, td);
    d3_ = a_dot(o.d0_, o.d1_, o.d2_, o.d3_, ta, tb, tc, td);
}

// | a0_ a1_ a2_ a3_ | |px|
// | b0_ b1_ b2_ b3_ | |py|
// | c0_ c1_ c2_ c3_ | |pz|
// | d0_ d1_ d2_ d3_ | |pw|
void Matrix44::multiply(Vector3* operand) const
{
    const double x = operand->x;
    const double y = operand->y;
    const double z = operand->z;
    const double w = operand->w;

    operand->x = a0_ * x + a1_ * y + a2_ * z + a3_ * w;
    operand->y = b0_ * x + b1_ * y + b2_ * z + b3_ * w;
    operand->z = c0_ * x + c1_ * y + c2_ * z + c3_ * w;
    operand->w = d0_ * x + d1_ * y + d2_ * z + d3_ * w;
}

// def. Z = (a + bz) / -z
// 0 = (a + -nb) / +n
// 1 = (a + -fb) / +f
// f = (n - f)b
// b = f / (n - f)
// a = nb
//
// | sk 0   0   0 | | a0_ a1_ a2_ a3_ |
// | 0  s   0   0 | | b0_ b1_ b2_ b3_ |
// | 0  0   b   a | | c0_ c1_ c2_ c3_ |
// | 0  0   -1  0 | | d0_ d1_ d2_ d3_ |
void Matrix44::perspective( double theta,
                            int width,
                            int height,
                            double near_clip,
                            double far_clip)
{
    const double c0 = c0_;
    const double c1 = c1_;
    const double c2 = c2_;
    const double c3 = c3_;

    const double s = 1.0 / GameLib::tan(theta / 2.0);
    const double k = static_cast< double >(height) / width;
    const double b = far_clip / (near_clip - far_clip);
    const double a = near_clip * b;

    a0_ = s * k * a0_;
    a1_ = s * k * a1_;
    a2_ = s * k * a2_;
    a3_ = s * k * a3_;

    b0_ = s * b0_;
    b1_ = s * b1_;
    b2_ = s * b2_;
    b3_ = s * b3_;

    c0_ = b * c0_ + a * d0_;
    c1_ = b * c1_ + a * d1_;
    c2_ = b * c2_ + a * d2_;
    c3_ = b * c3_ + a * d3_;

    d0_ = -c0;
    d1_ = -c1;
    d2_ = -c2;
    d3_ = -c3;
}

void Matrix44::rotate(const Vector3& angle)
{
    rotate_zx(angle.y);
    rotate_yz(angle.x);
    rotate_xy(angle.z);
}

// |x|   | 1    0           0           0 | |px|
// |y| = | 0    cos(theta)  -sin(theta) 0 | |py|
// |z|   | 0    sin(theta)  cos(theta)  0 | |pz|
// |w|   | 0    0           0           1 | |pw|
//
// | 1    0           0           0 | | a0_ a1_ a2_ a3_ |
// | 0    cos(theta)  -sin(theta) 0 | | b0_ b1_ b2_ b3_ |
// | 0    sin(theta)  cos(theta)  0 | | c0_ c1_ c2_ c3_ |
// | 0    0           0           1 | | d0_ d1_ d2_ d3_ |
void Matrix44::rotate_yz(double angle)
{
    const double cosine = GameLib::cos(angle);
    const double sine = GameLib::sin(angle);

    const double b0 = b0_;
    const double b1 = b1_;
    const double b2 = b2_;
    const double b3 = b3_;

    b0_ = cosine * b0 - sine * c0_;
    b1_ = cosine * b1 - sine * c1_;
    b2_ = cosine * b2 - sine * c2_;
    b3_ = cosine * b3 - sine * c3_;

    c0_ = sine * b0 + cosine * c0_;
    c1_ = sine * b1 + cosine * c1_;
    c2_ = sine * b2 + cosine * c2_;
    c3_ = sine * b3 + cosine * c3_;
}

// |x|   | cos(theta)  0   sin(theta)  0 | |px|
// |y| = | 0           1   0           0 | |py|
// |z|   | -sin(theta) 0   cos(theta)  0 | |pz|
// |w|   | 0           0   0           1 | |pw|
//
// | cos(theta)    0   sin(theta)  0 | | a0_ a1_ a2_ a3_ |
// | 0             1   0           0 | | b0_ b1_ b2_ b3_ |
// | -sin(theta)   0   cos(theta)  0 | | c0_ c1_ c2_ c3_ |
// | 0             0   0           1 | | d0_ d1_ d2_ d3_ |
void Matrix44::rotate_zx(double angle)
{
    const double cosine = GameLib::cos(angle);
    const double sine = GameLib::sin(angle);

    const double a0 = a0_;
    const double a1 = a1_;
    const double a2 = a2_;
    const double a3 = a3_;

    a0_ = cosine * a0 + sine * c0_;
    a1_ = cosine * a1 + sine * c1_;
    a2_ = cosine * a2 + sine * c2_;
    a3_ = cosine * a3 + sine * c3_;

    c0_ = -sine * a0 + cosine * c0_;
    c1_ = -sine * a1 + cosine * c1_;
    c2_ = -sine * a2 + cosine * c2_;
    c3_ = -sine * a3 + cosine * c3_;
}

// |x|   | cos(theta)  -sin(theta)  0   0 | |px|
// |y|   | sin(theta)  cos(theta)   0   0 | |py|
// |z| = | 0           0            1   0 | |pz|
// |w|   | 0           0            0   1 | |pw|
//
// | cos(theta)    -sin(theta)  0   0 | | a0_ a1_ a2_ a3_ |
// | sin(theta)    cos(theta)   0   0 | | b0_ b1_ b2_ b3_ |
// | 0             0            1   0 | | c0_ c1_ c2_ c3_ |
// | 0             0            0   1 | | d0_ d1_ d2_ d3_ |
void Matrix44::rotate_xy(double angle)
{
    const double cosine = GameLib::cos(angle);
    const double sine = GameLib::sin(angle);

    const double a0 = a0_;
    const double a1 = a1_;
    const double a2 = a2_;
    const double a3 = a3_;

    a0_ = cosine * a0 - sine * b0_;
    a1_ = cosine * a1 - sine * b1_;
    a2_ = cosine * a2 - sine * b2_;
    a3_ = cosine * a3 - sine * b3_;

    b0_ = sine * a0 + cosine * b0_;
    b1_ = sine * a1 + cosine * b1_;
    b2_ = sine * a2 + cosine * b2_;
    b3_ = sine * a3 + cosine * b3_;
}

// | 1  0   0   delta.x | | a0_ a1_ a2_ a3_ |
// | 0  1   0   delta.y | | b0_ b1_ b2_ b3_ |
// | 0  0   1   delta.z | | c0_ c1_ c2_ c3_ |
// | 0  0   0   1       | | d0_ d1_ d2_ d3_ |
void Matrix44::translate(const Vector3& delta)
{
    a0_ = a0_ + delta.x * d0_;
    a1_ = a1_ + delta.x * d1_;
    a2_ = a2_ + delta.x * d2_;
    a3_ = a3_ + delta.x * d3_;

    b0_ = b0_ + delta.y * d0_;
    b1_ = b1_ + delta.y * d1_;
    b2_ = b2_ + delta.y * d2_;
    b3_ = b3_ + delta.y * d3_;

    c0_ = c0_ + delta.z * d0_;
    c1_ = c1_ + delta.z * d1_;
    c2_ = c2_ + delta.z * d2_;
    c3_ = c3_ + delta.z * d3_;
}


// | a  0   0   0 | | a0_ a1_ a2_ a3_ |
// | 0  a   0   0 | | b0_ b1_ b2_ b3_ |
// | 0  0   a   0 | | c0_ c1_ c2_ c3_ |
// | 0  0   0   1 | | d0_ d1_ d2_ d3_ |
void Matrix44::scale(double a)
{
    a0_ = a * a0_;
    a1_ = a * a1_;
    a2_ = a * a2_;
    a3_ = a * a3_;
    b0_ = a * b0_;
    b1_ = a * b1_;
    b2_ = a * b2_;
    b3_ = a * b3_;
    c0_ = a * c0_;
    c1_ = a * c1_;
    c2_ = a * c2_;
    c3_ = a * c3_;
}

} // namespace GraphicsDatabase
