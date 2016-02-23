#include "GraphicsDatabase/Camera.h"
#include <cmath>
#include "GameLib/Framework.h"
#include "GraphicsDatabase/Matrix44.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

Camera::Camera(int width, int height, double near_clip, double far_clip)
:   near_clip_(near_clip), far_clip_(far_clip),
    width_(width), height_(height),
    angle_of_view_(90.0),
    position_(0.0, 0.0, 0.0),
    angle_(0.0, 0.0, 0.0)
{}

Camera::~Camera() {}

double Camera::near_clip() const { return near_clip_; }

double Camera::far_clip() const { return far_clip_; }

double Camera::angle_of_view() const { return angle_of_view_; }

void Camera::angle_of_view(double new_value) { angle_of_view_ = new_value; }

const Vector3* Camera::position() const { return &position_; }

void Camera::position(const Vector3& new_value)
{
    position_.copy_from(new_value);
}

const Vector3* Camera::angle() const { return &angle_; }

void Camera::angle(const Vector3& new_value)
{
    angle_.copy_from(new_value);
}

Matrix44 Camera::get_perspective_matrix() const
{
    Matrix44 perspective;
    perspective.translate(-position_);
    perspective.rotate_zx(angle_.y);
    perspective.rotate_yz(angle_.x);
    perspective.rotate_xy(angle_.z);
    perspective.perspective(    angle_of_view_,
                                width_,
                                height_,
                                near_clip_,
                                far_clip_);
    return perspective;
}

} // namespace GraphicsDatabase
