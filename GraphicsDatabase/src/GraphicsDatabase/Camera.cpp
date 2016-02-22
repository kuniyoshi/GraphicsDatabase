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
    angle_yz_(0.0), angle_zx_(0.0), angle_xy_(0.0),
    angle_of_view_(90.0),
    position_(0.0, 0.0, 0.0)
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

Matrix44 Camera::get_perspective_matrix() const
{
    Matrix44 perspective;
    perspective.translate(-position_);
    perspective.rotate_zx(angle_zx_);
    perspective.rotate_yz(angle_yz_);
    perspective.rotate_xy(angle_xy_);
    perspective.perspective(    angle_of_view_,
                                width_,
                                height_,
                                near_clip_,
                                far_clip_);
    return perspective;
}

void Camera::rotate(const Vector3& diff)
{
    angle_yz_ = angle_yz_ + diff.x;
    angle_zx_ = angle_zx_ + diff.y;
    angle_xy_ = angle_xy_ + diff.z;
}

} // namespace GraphicsDatabase
