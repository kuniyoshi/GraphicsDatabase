#include "GraphicsDatabase/Animation.h"
#include <cassert>
#include <cmath>
#include <string>
#include <utility>
#include <vector>
#include "GraphicsDatabase/TheCompletion.h"
#include "GraphicsDatabase/Vector3.h"

using std::pair;

namespace GraphicsDatabase
{

Animation::Animation(const std::string& id)
:   id_(id),
    period_(1.0),
    scales_(0), scales_size_(0),
    angles_(0), angles_size_(0),
    positions_(0), positions_size_(0),
    completion_method_(TheCompletion::MethodLastOne)
{}

Animation::~Animation()
{
    if (scales_)
    {
        delete[] scales_;
        scales_ = 0;
    }

    if (angles_)
    {
        delete[] angles_;
        angles_ = 0;
    }

    if (positions_)
    {
        delete[] positions_;
        positions_ = 0;
    }
}

void Animation::period(double new_value)
{
    period_ = new_value;
}

void Animation::scales(const std::vector< double >& scales)
{
    assert((scales.size() % 2) == 0);
    assert(!scales_);
    scales_size_ = scales.size() / 2;
    scales_ = new pair< double, double >[scales_size_];

    for (size_t i = 0; i < scales_size_; ++i)
    {
        scales_[i].first = scales[2 * i];
        scales_[i].second = scales[2 * i + 1];
    }
}

void Animation::angles(char axis, const std::vector< double >& angles)
{
    assert((angles.size() % 2) == 0);

    if (!angles_size_)
    {
        angles_size_ = angles.size() / 2;
    }

    assert((angles.size() / 2) == angles_size_);

    if (!angles_)
    {
        angles_ = new pair< double, Vector3 >[angles_size_];
    }

    int index = 0;

    switch (axis)
    {
        case 'x': index = 0; break;
        case 'y': index = 1; break;
        case 'z': index = 2; break;
        default: assert(false); break;
    }

    for (size_t i = 0; i < angles_size_; ++i)
    {
        angles_[i].first = angles[2 * i];
        double* xyz = &(angles_[i].second.x);
        xyz[index] = angles[2 * i + 1];
    }
}

void Animation::positions(char axis, const std::vector< double >& positions)
{
    assert((positions.size() % 2) == 0);

    if (!positions_size_)
    {
        positions_size_ = positions.size() / 2;
    }

    assert((positions.size() / 2) == positions_size_);

    if (!positions_)
    {
        positions_ = new pair< double, Vector3 >[positions_size_];
    }

    int index = 0;

    switch (axis)
    {
        case 'x': index = 0; break;
        case 'y': index = 0; break;
        case 'z': index = 0; break;
        default: assert(false); break;
    }

    for (size_t i = 0; i < positions_size_; ++i)
    {
        positions_[i].first = positions[2 * i];
        double *xyz = &(positions_[i].second.x);
        xyz[index] = positions[2 * i + 1];
    }
}

void Animation::completion_method(const std::string& method_id)
{
    if (method_id == "last_one")
    {
        completion_method_ = TheCompletion::MethodLastOne;
    }
    else if (method_id == "linear")
    {
        completion_method_ = TheCompletion::MethodLinear;
    }
    else
    {
        completion_method_ = TheCompletion::MethodLastOne;
    }
}

double Animation::scale_at(double time) const
{
    double rate = std::fmod(time, period_) / period_;
    return TheCompletion::complete( completion_method_,
                                    rate,
                                    scales_,
                                    scales_size_);
}

Vector3 Animation::angle_at(double time) const
{
    double rate = std::fmod(time, period_) / period_;
    return TheCompletion::complete( completion_method_,
                                    rate,
                                    angles_,
                                    angles_size_);
}

Vector3 Animation::position_at(double time) const
{
    double rate = std::fmod(time, period_) / period_;
    return TheCompletion::complete( completion_method_,
                                    rate,
                                    positions_,
                                    positions_size_);
}

bool Animation::has_scale_completion() const { return !!scales_; }

bool Animation::has_angle_completion() const { return !!angles_; }

bool Animation::has_position_completion() const { return !!positions_; }

} // namespace GraphicsDatabase
