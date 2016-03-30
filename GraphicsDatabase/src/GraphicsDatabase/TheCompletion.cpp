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

// [[t, a, b, c], ...]
void complete_polynomial2(  double* completed,
                            const double rate,
                            const double* t_and_data,
                            const size_t size)
{
    const size_t t_size = size / 4;

    for (size_t i = 0; i < t_size; ++i)
    {
        const double* a_data = &(t_and_data[4 * i]);

        if (a_data[0] > rate)
        {
            const double* previous_data = &(t_and_data[4 * (i - 1)]);
            const double a = previous_data[1];
            const double b = previous_data[2];
            const double c = previous_data[3];
            *completed = a + b * rate + c * rate * rate;
            return;
        }
    }
}

// [[t, a, b, c, d], ...]
void complete_polynomial3(  double* completed,
                            const double rate,
                            const double* t_and_data,
                            const size_t size)
{
    const size_t t_size = size / 5;

    for (size_t i = 0; i < t_size - 1; ++i)
    {
        const double* a_data = &(t_and_data[5 * i]);

        if (a_data[0] > rate)
        {
            const double* previous_data = &(t_and_data[5 * (i - 1)]);
            const double t0 = previous_data[0];
            const double a = previous_data[1];
            const double b = previous_data[2];
            const double c = previous_data[3];
            const double d = previous_data[4];
            const double u = (rate - t0) / (a_data[0] - t0);
            // *completed = a + b * u + c * u * u + d * u * u * u;
            *completed = c + d * u;
            *completed *= u;
            *completed += b;
            *completed *= u;
            *completed += a;
            return;
        }
    }

    {
        const double* a_data = 0;

        if (t_and_data[5 * (t_size - 1)] == 1.0)
        {
            a_data = &(t_and_data[5 * (t_size - 2)]);
        }
        else
        {
            a_data = &(t_and_data[5 * (t_size - 1)]);
        }

        const double t = a_data[0];
        const double a = a_data[1];
        const double b = a_data[2];
        const double c = a_data[3];
        const double d = a_data[4];
        const double u = (rate - t) / (1.0 - t);
        *completed = c + d * u;
        *completed *= u;
        *completed += b;
        *completed *= u;
        *completed += a;
        return;
    }
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

        case MethodPolynomial2:
        complete_polynomial2(   completed,
                                rate,
                                t_and_data,
                                total_size);
        return;
        break;

        case MethodPolynomial3:
        complete_polynomial3(   completed,
                                rate,
                                t_and_data,
                                total_size);
        return;
        break;
    }

    assert(!"Unknown method found.");
}

} // namespace TheCompletion

} // namespace GraphicsDatabase
