#include "GraphicsDatabase/Animation.h"
#include <cassert>
#include <string>
#include <vector>
#include "GraphicsDatabase/Completion.h"
#include "GraphicsDatabase/TheCompletion.h"
#include "GraphicsDatabase/Vector3.h"

using std::pair;

namespace GraphicsDatabase
{

Animation::Animation(const std::string& id)
:   id_(id),
    scale_completion_(0),
    angle_completion_(0),
    position_completion_(0)
{}

Animation::~Animation()
{
    if (scale_completion_)
    {
        delete scale_completion_;
        scale_completion_ = 0;
    }

    if (angle_completion_)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (angle_completion_[i])
            {
                delete angle_completion_[i];
                angle_completion_[i] = 0;
            }
        }

        delete[] angle_completion_;
        angle_completion_ = 0;
    }

    if (position_completion_)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (position_completion_[i])
            {
                delete position_completion_[i];
                position_completion_[i] = 0;
            }
        }

        delete[] position_completion_;
        position_completion_= 0;
    }
}

void Animation::scale_completion(   const std::string& completion_id,
                                    const std::vector< double >& scales,
                                    const double period)
{
    assert((scales.size() % 2) == 0);
    assert(!scale_completion_);
    scale_completion_ = new Completion( completion_id,
                                        scales,
                                        period);
}

bool Animation::has_scale_completion() const
{
    return !!scale_completion_;
}

void Animation::angle_completion(   const std::string& completion_id,
                                    const char axis,
                                    const std::vector< double >& angles,
                                    const double period)
{
    assert((angles.size() % 2) == 0);

    if (!angle_completion_)
    {
        angle_completion_ = new Completion*[3];
        angle_completion_[0] = 0;
        angle_completion_[1] = 0;
        angle_completion_[2] = 0;
    }

    int index = 0;

    switch (axis)
    {
        case 'x': index = 0; break;
        case 'y': index = 1; break;
        case 'z': index = 2; break;
        default: assert(false); break;
    }

    assert(!angle_completion_[index]);

    angle_completion_[index] = new Completion(  completion_id,
                                                angles,
                                                period);
}

bool Animation::has_angle_completion() const
{
    return !!angle_completion_;
}

void Animation::position_completion(    const std::string& completion_id,
                                        char axis,
                                        const std::vector< double >& positions,
                                        const double period)
{
    assert((positions.size() % 2) == 0);

    if (!position_completion_)
    {
        position_completion_ = new Completion*[3];
        position_completion_[0] = 0;
        position_completion_[1] = 0;
        position_completion_[2] = 0;
    }

    int index = 0;

    switch (axis)
    {
        case 'x': index = 0; break;
        case 'y': index = 1; break;
        case 'z': index = 2; break;
        default: assert(false); break;
    }

    assert(!position_completion_[index]);

    position_completion_[index] = new Completion(   completion_id,
                                                    positions,
                                                    period);
}

bool Animation::has_position_completion() const
{
    return !!position_completion_;
}

void Animation::scale_at(double* scale, const double time) const
{
    assert(scale_completion_);
    scale_completion_->complete(scale, time);
}

void Animation::angle_at(Vector3* angle, const double time) const
{
    assert(angle_completion_);

    for (int i = 0; i < 3; ++i)
    {
        if (angle_completion_[i])
        {
            double* completed = &(angle->x);
            angle_completion_[i]->complete(&(completed[i]), time);
        }
    }
}

void Animation::position_at(Vector3* position, const double time) const
{
    assert(position_completion_);

    for (int i = 0; i < 3; ++i)
    {
        if (position_completion_[i])
        {
            double* completed = &(position->x);
            position_completion_[i]->complete(&(completed[i]), time);
        }
    }
}

} // namespace GraphicsDatabase
