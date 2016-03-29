#include "GraphicsDatabase/TheCompletion.h"
#include <cassert>

namespace GraphicsDatabase
{

namespace TheCompletion
{

namespace
{

// t_and_data = [[t, point], [t, point], ...]
void complete_last_one( double* completed,
                        const double rate,
                        const double* t_and_data,
                        const size_t size)
{
    size_t index = 0;
    *completed = t_and_data[1];

    for (size_t i = 2; i < size; i += 2)
    {
        if (t_and_data[i] >= rate)
        {
            *completed = t_and_data[i + 1];
            return;
        }
    }
}

void complete_linear(   double* value,
                        const double rate,
                        const double* t_and_data,
                        const size_t size)
{
    if (rate == t_and_data[0])
    {
        *value = t_and_data[0 + 1];
        return;
    }

    // loop completion
    if (rate < t_and_data[0])
    {
        const double t0 = t_and_data[size - 2];
        const double t1 = t_and_data[0];
        const double a0 = t_and_data[size - 1];
        const double a1 = t_and_data[1];
        const double dt = (1.0 - t0 + rate) / (1.0 - t0 + t1);
        const double da = a1 - a0;
        *value = a0 + da * dt;
        return;
    }

    for (size_t i = 0; i < size; i += 2)
    {
        if (t_and_data[i] > rate)
        {
            const double t0 = t_and_data[i - 2];
            const double t1 = t_and_data[i];
            const double a0 = t_and_data[i - 1];
            const double a1 = t_and_data[i + 1];
            const double dt = (rate - t0) / (t1 - t0);
            const double da = a1 - a0;
            *value = a0 + da * dt;
            return;
        }
    }

    // loop completion
    const double t0 = t_and_data[size - 2];
    const double t1 = t_and_data[0];
    const double a0 = t_and_data[size - 1];
    const double a1 = t_and_data[1];
    const double dt = (1.0 - rate) / (1.0 - t0 + t1);
    const double da = a1 - a0;
    *value = a0 + da * dt;
    return;
}

} // namespace -

void complete(  double* completed,
                const Method method,
                const double rate,
                const double* t_and_data,
                const size_t total_size,
                const size_t size_per_t)
{
    assert(rate >= 0.0 && rate <= 1.0);

    switch (method)
    {
        case MethodLastOne:
        complete_last_one(  completed,
                            rate,
                            t_and_data,
                            total_size);
        return;
        break;

        case MethodLinear:
        complete_linear(    completed,
                            rate,
                            t_and_data,
                            total_size);
        return;
        break;
    }

    assert(false);
}

} // namespace TheCompletion

} // namespace GraphicsDatabase
