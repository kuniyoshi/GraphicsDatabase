#include "GraphicsDatabase/Camera.h"
#include <cmath>
#include "GameLib/Framework.h"
#include "GraphicsDatabase/Matrix44.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

namespace
{

const double NearClip       = 1.0;
const double FarClip        = 1000.0;
const int Width             = 100;
const int Height            = 100;
const double AngleOfView    = 90.0;
const Vector3 Position(0.0, 0.0, 0.0);
const Vector3 Angle(0.0, 0.0, 0.0);

} // namespace -

Camera::Camera()
:   near_clip_(NearClip), far_clip_(FarClip),
    width_(Width), height_(Height),
    angle_of_view_(AngleOfView),
    position_(Position),
    angle_(Angle)
{}

Camera::Camera(int width, int height, double near_clip, double far_clip)
:   near_clip_(near_clip), far_clip_(far_clip),
    width_(width), height_(height),
    angle_of_view_(AngleOfView),
    position_(Position),
    angle_(Angle)
{}

Camera::~Camera() {}

double Camera::near_clip() const { return near_clip_; }

void Camera::near_clip(double new_value) { near_clip_ = new_value; }

double Camera::far_clip() const { return far_clip_; }

void Camera::far_clip(double new_value) { far_clip_ = new_value; }

double Camera::angle_of_view() const { return angle_of_view_; }

void Camera::angle_of_view(double new_value) { angle_of_view_ = new_value; }

int Camera::width() const { return width_; }

void Camera::width(int new_value) { width_ = new_value; }

int Camera::height() const { return height_; }

void Camera::height(int new_value) { height_ = new_value; }

const Vector3& Camera::position() const { return position_; }

void Camera::position(const Vector3& new_value)
{
    position_.copy_from(new_value);
}

const Vector3& Camera::angle() const { return angle_; }

void Camera::angle(const Vector3& new_value)
{
    angle_.copy_from(new_value);
}

Matrix44 Camera::get_perspective_matrix() const
{
    Matrix44 perspective;
    perspective.translate(-position_);
    perspective.rotate(angle_);
    perspective.perspective(    angle_of_view_,
                                width_,
                                height_,
                                near_clip_,
                                far_clip_);
    return perspective;
}

} // namespace GraphicsDatabase
