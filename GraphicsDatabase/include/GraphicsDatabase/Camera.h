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
    double angle_of_view_;
    int width_;
    int height_;
    Vector3 position_;
    Vector3 angle_;

public:
    Camera();
    Camera(int width, int height, double near_clip, double far_clip);
    ~Camera();
    double near_clip() const;
    void near_clip(double new_value);
    double far_clip() const;
    void far_clip(double new_value);
    double angle_of_view() const;
    void angle_of_view(double new_value);
    int width() const;
    void width(int new_value);
    int height() const;
    void height(int new_value);
    const Vector3* position() const;
    void position(const Vector3& new_value);
    const Vector3* angle() const;
    void angle(const Vector3& new_value);
    Matrix44 get_perspective_matrix() const;
};

} // namespace GraphicsDatabase

#endif
