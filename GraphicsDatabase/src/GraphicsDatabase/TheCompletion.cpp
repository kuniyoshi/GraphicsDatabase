#include "GraphicsDatabase/TheCompletion.h"
#include <utility>
#include "GraphicsDatabase/Vector3.h"

using std::pair;

namespace GraphicsDatabase
{

namespace TheCompletion
{

namespace
{

void complete_last_one( double* value,
                        double rate,
                        const pair< double, double >* points,
                        size_t size)
{
    size_t index = 0;
    *value = points[0].second;

    for (size_t i = 1; i < size; ++i)
    {
        if (points[i].first >= rate)
        {
            *value = points[i - 1].second;
            break;
        }
    }
}

void complete_linear(   double* value,
                        double rate,
                        const pair< double, double >* points,
                        size_t size)
{
    if (rate == points[0].first)
    {
        *value = points[0].second;
        return;
    }

    // loop completion
    if (rate < points[0].first)
    {
        const double lsat_to_end = 1.0 - points[size - 1].first;
        const double dt = lsat_to_end + points[0].first;
        const double t = (lsat_to_end + rate) / dt;
        const double da = points[0].second - points[size - 1].second;
        *value= points[size - 1].second
        + (points[0].second - points[size - 1].second) * t;
        return;
    }

    for (size_t i = 0; i < size; ++i)
    {
        if (points[i].first == rate)
        {
            *value = points[i].second;
            return;
        }

        if (points[i].first > rate)
        {
            const double dt = points[i].first - points[i - 1].first;
            const double t = (rate - points[i - 1].first) / dt;
            const double da = points[i].second - points[i - 1].second;
            * value = points[i - 1].second + da * t;
            return;
        }
    }

    // loop completion
    const double dt = (1.0 - points[size - 1].first) + points[0].first;
    const double t = (rate - points[size - 1].first) / dt;
    const double da = points[0].second - points[size - 1].second;
    *value = points[size - 1].second + da * t;
    return;
}

void complete_last_one( Vector3* value,
                        double rate,
                        const pair< double, Vector3 >* points,
                        size_t size)
{
    size_t index = 0;
    value->copy_from(points[0].second);

    for (size_t i = 1; i < size; ++i)
    {
        if (points[i].first >= rate)
        {
            value->copy_from(points[i - 1].second);
            break;
        }
    }
}

void complete_linear_one(   Vector3* value,
                            double t,
                            const Vector3& from,
                            const Vector3& to)
{
    value->x = from.x + (to.x - from.x) * t;
    value->y = from.y + (to.y - from.y) * t;
    value->z = from.z + (to.z - from.z) * t;
}

void complete_linear(   Vector3* value,
                        double rate,
                        const pair< double, Vector3 >* points,
                        size_t size)
{
    if (rate == points[0].first)
    {
        value->copy_from(points[0].second);
    }

    // loop completion
    if (rate < points[0].first)
    {
        const double lsat_to_end = 1.0 - points[size - 1].first;
        const double dt = lsat_to_end + points[0].first;
        const double t = (lsat_to_end + rate) / dt;
        complete_linear_one(    value,
                                t,
                                points[size - 1].second,
                                points[0].second);
        return;
    }

    for (size_t i = 1; i < size; ++i)
    {
        if (points[i].first == rate)
        {
            value->copy_from(points[i].second);
            return;
        }

        if (points[i].first > rate)
        {
            const double dt = points[i].first - points[i - 1].first;
            const double t = (rate - points[i - 1].first) / dt;
            complete_linear_one(    value,
                                    t,
                                    points[i - 1].second,
                                    points[i].second);
            return;
        }
    }

    // loop completion
    const double dt = 1.0 - points[size - 1].first + points[0].first;
    const double t = (1.0 - rate) / dt;
    complete_linear_one(    value,
                            t,
                            points[size - 1].second,
                            points[0].second);
    return;
}

} // namespace -

double complete(    Method method,
                    double rate,
                    const pair< double, double >* points,
                    size_t size)
{
    double value;

    switch (method)
    {
        case MethodLastOne:
        complete_last_one(&value, rate, points, size);
        break;

        case MethodLinear:
        complete_linear(&value, rate, points, size);
        break;

        default:
        complete_last_one(&value, rate, points, size);
        break;
    }

    return value;
}

Vector3 complete(   Method method,
                    double rate,
                    const pair< double, Vector3 >* points,
                    size_t size)
{
    Vector3 value;

    switch (method)
    {
        case MethodLastOne:
        complete_last_one(&value, rate, points, size);
        break;

        case MethodLinear:
        complete_linear(&value, rate, points, size);
        break;

        default:
        complete_last_one(&value, rate, points, size);
        break;
    }

    return value;
}

} // namespace TheCompletion

} // namespace GraphicsDatabase
