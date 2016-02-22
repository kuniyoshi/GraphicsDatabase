#ifndef GRAPHICS_DATABASE__CAMERA_H_
#define GRAPHICS_DATABASE__CAMERA_H_
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

class Matrix44;

class Camera
{
private:
    double near_clip_;
    double far_clip_;
    double angle_yz_;
    double angle_zx_;
    double angle_xy_;
    double angle_of_view_;
    int width_;
    int height_;
    Vector3 position_;

public:
    Camera(int width, int height, double near_clip, double far_clip);
    ~Camera();
    double near_clip() const;
    double far_clip() const;
    double angle_of_view() const;
    void angle_of_view(double new_value);
    const Vector3* position() const;
    void position(const Vector3& new_value);
    Matrix44 get_perspective_matrix() const;
    void rotate(const Vector3& diff);
};

} // namespace GraphicsDatabase

#endif
