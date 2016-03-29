#ifndef GRAPHICS_DATABASE__COMPLETION_H_
#define GRAPHICS_DATABASE__COMPLETION_H_
#include <string>
#include <vector>
#include "GraphicsDatabase/TheCompletion.h"

namespace GraphicsDatabase
{

class Vector3;

class Completion
{
private:
    double* t_and_data_;
    size_t total_size_;
    size_t size_per_t_;
    double period_;
    TheCompletion::Method method_;

public:
    Completion( const std::string& completion_id,
                const std::vector< double >& data,
                const double period);
    ~Completion();
    void complete(double* completed, const double t) const;
    void transform_polynomial2(const double t, const double speed);
};

} // namespace GraphicsDatabase

#endif
