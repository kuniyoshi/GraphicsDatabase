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

namespace
{

int index_of(const char axis)
{
    switch (axis)
    {
        case 'x': return 0; break;
        case 'y': return 1; break;
        case 'z': return 2; break;
        default: assert(!"Unknown axis found."); break;
    }
}

} // namespace -

Animation::Animation(const std::string& id)
:   id_(id),
    scale_completions_(0),
    angle_completions_(0),
    position_completions_(0)
{}

Animation::~Animation()
{
    if (scale_completions_)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (scale_completions_[i])
            {
                delete scale_completions_[i];
                scale_completions_[i] = 0;
            }
        }

        delete[] scale_completions_;
        scale_completions_ = 0;
    }

    if (angle_completions_)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (angle_completions_[i])
            {
                delete angle_completions_[i];
                angle_completions_[i] = 0;
            }
        }

        delete[] angle_completions_;
        angle_completions_ = 0;
    }

    if (position_completions_)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (position_completions_[i])
            {
                delete position_completions_[i];
                position_completions_[i] = 0;
            }
        }

        delete[] position_completions_;
        position_completions_= 0;
    }
}

void Animation::scale_completions(  const std::string& completion_id,
                                    const char axis,
                                    const std::vector< double >& scales,
                                    const double period)
{
    if (!scale_completions_)
    {
        scale_completions_ = new Completion*[3];
        scale_completions_[0] = 0;
        scale_completions_[1] = 0;
        scale_completions_[2] = 0;
    }

    int index = index_of(axis);

    assert(!scale_completions_[index]);

    scale_completions_[index] = new Completion( completion_id,
                                                scales,
                                                period);
}

bool Animation::has_scale_completions() const
{
    return !!scale_completions_;
}

void Animation::angle_completions(  const std::string& completion_id,
                                    const char axis,
                                    const std::vector< double >& angles,
                                    const double period)
{
    if (!angle_completions_)
    {
        angle_completions_ = new Completion*[3];
        angle_completions_[0] = 0;
        angle_completions_[1] = 0;
        angle_completions_[2] = 0;
    }

    int index = index_of(axis);

    assert(!angle_completions_[index]);

    angle_completions_[index] = new Completion( completion_id,
                                                angles,
                                                period);
}

bool Animation::has_angle_completions() const
{
    return !!angle_completions_;
}

void Animation::position_completions(   const std::string& completion_id,
                                        char axis,
                                        const std::vector< double >& positions,
                                        const double period)
{
    assert((positions.size() % 2) == 0);

    if (!position_completions_)
    {
        position_completions_ = new Completion*[3];
        position_completions_[0] = 0;
        position_completions_[1] = 0;
        position_completions_[2] = 0;
    }

    int index = index_of(axis);

    assert(!position_completions_[index]);

    position_completions_[index] = new Completion(  completion_id,
                                                    positions,
                                                    period);
}

bool Animation::has_position_completions() const
{
    return !!position_completions_;
}

void Animation::apply_speed_to_angle(   const char axis,
                                        const double t,
                                        const double speed)
{
    assert(angle_completions_);
    int index = index_of(axis);
    assert(angle_completions_[index]);
    angle_completions_[index]->transform_polynomial2(t, speed);
}

void Animation::scale_at(Vector3* scale, const double time) const
{
    assert(scale_completions_);

    for (int i = 0; i < 3; ++i)
    {
        if (scale_completions_[i])
        {
            double* completed = &(scale->x);
            scale_completions_[i]->complete(&(completed[i]), time);
        }
    }
}

void Animation::angle_at(Vector3* angle, const double time) const
{
    assert(angle_completions_);

    for (int i = 0; i < 3; ++i)
    {
        if (angle_completions_[i])
        {
            double* completed = &(angle->x);
            angle_completions_[i]->complete(&(completed[i]), time);
        }
    }
}

void Animation::position_at(Vector3* position, const double time) const
{
    assert(position_completions_);

    for (int i = 0; i < 3; ++i)
    {
        if (position_completions_[i])
        {
            double* completed = &(position->x);
            position_completions_[i]->complete(&(completed[i]), time);
        }
    }
}

void Animation::transform_angle_polynomial3(const char axis)
{
    assert(angle_completions_);
    int index = index_of(axis);
    assert(angle_completions_[index]);
    angle_completions_[index]->transform_polynomial3();
}

} // namespace GraphicsDatabase
