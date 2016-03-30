#ifndef GRAPHICS_DATABASE__THE_COMPLETION_H_
#define GRAPHICS_DATABASE__THE_COMPLETION_H_
#include <utility>

using std::pair;

namespace GraphicsDatabase
{

class Vector3;

namespace TheCompletion
{

enum Method
{
    MethodLastOne,
    MethodLinear,
    MethodPolynomial2,
    MethodPolynomial3,
};

void complete(  double* completed,
                const Method method,
                const double rate,
                const double* t_and_data,
                const size_t total_size,
                const size_t size_per_t);

} // namespace TheCompletion

} // namespace GraphicsDatabase

#endif
