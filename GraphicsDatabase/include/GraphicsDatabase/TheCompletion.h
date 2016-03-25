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
};

double complete(    Method method,
                    double rate,
                    const pair< double, double >* points,
                    size_t size);

Vector3 complete(   Method method,
                    double rate,
                    const pair< double, Vector3 >* points,
                    size_t size);

} // namespace TheCompletion

} // namespace GraphicsDatabase

#endif
