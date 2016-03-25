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

        default:
        complete_last_one(&value, rate, points, size);
        break;
    }

    return value;
}

} // namespace TheCompletion

} // namespace GraphicsDatabase
