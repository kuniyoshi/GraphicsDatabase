#include "GraphicsDatabase/Completion.h"
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include "GraphicsDatabase/TheCompletion.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

namespace
{

TheCompletion::Method get_method_from_id(const std::string& id)
{
    if (id == "last_one")
    {
        return TheCompletion::MethodLastOne;
    }
    else if (id == "linear")
    {
        return TheCompletion::MethodLinear;
    }
    else if (id == "polynomial2")
    {
        return TheCompletion::MethodPolynomial2;
    }

    assert(!"Unknown method id found.");
}

size_t get_chunk_size(const TheCompletion::Method method)
{
    switch (method)
    {
        case TheCompletion::MethodLastOne: return 1; break;
        case TheCompletion::MethodLinear: return 1; break;
        case TheCompletion::MethodPolynomial2: return 3; break;
        default: assert(!"Unknown method found."); break;
    }

    assert(!"Unknown method found.");
}

} // namespace -

Completion::Completion( const std::string& completion_id,
                        const std::vector< double >& data,
                        const double period)
:   t_and_data_(0),
    total_size_(0),
    size_per_t_(0),
    period_(period),
    method_(TheCompletion::MethodLastOne)
{
    method_ = get_method_from_id(completion_id);
    total_size_ = data.size();
    size_per_t_ = get_chunk_size(method_);
    t_and_data_ = new double[total_size_];

    for (size_t i = 0; i < total_size_; ++i)
    {
        t_and_data_[i] = data[i];
    }
}

Completion::~Completion()
{
    delete[] t_and_data_;
    t_and_data_ = 0;
}

void Completion::complete(double* completed, const double t) const
{
    const double rate = std::fmod(t, period_) / period_;
    TheCompletion::complete(    completed,
                                method_,
                                rate,
                                t_and_data_,
                                total_size_,
                                size_per_t_);
}

// p = a + b t + c t^2
void Completion::transform_polynomial2(const double t, const double speed)
{
    assert(t_and_data_);
    assert(size_per_t_ == 3);
    assert((total_size_ % 2) == 0);

    int index = -1;
    bool did_index_find = false;

    for (size_t i = 0; i < total_size_ / 2; ++i)
    {
        if (t_and_data_[2 * i] == t)
        {
            index = i;
            did_index_find = true;
        }
    }

    assert(did_index_find);

    size_t t_size = total_size_ / 2;
    const size_t chunk_size = size_per_t_ + 1;
    total_size_ = t_size * chunk_size;

    double* new_data = new double[total_size_];

    double v0 = speed;

    for (size_t i = index; i < t_size - 1; ++i)
    {
        const double t0 = t_and_data_[2 * i];
        const double t1 = t_and_data_[2 * (i + 1)];
        const double p0 = t_and_data_[2 * i + 1];
        const double p1 = t_and_data_[2 * (i + 1) + 1];
        const double t1mt0 = t1 - t0;
        const double t1mt02 = t1mt0 * t1mt0;
        const double c = (p1 - p0 - v0 * t1mt0) / t1mt02;
        const double b = v0 - 2 * c * t0;
        const double a = p0 - b * t0 - c * t0 * t0;
        double* a_data = &(new_data[i * chunk_size]);
        a_data[0] = t0;
        a_data[1] = a;
        a_data[2] = b;
        a_data[3] = c;

        v0 = b + 2 * c * t1;
    }

    double v1 = speed;

    for (size_t i = index; i > 0; --i)
    {
        const double t1 = t_and_data_[2 * i];
        const double t0 = t_and_data_[2 * (i - 1)];
        const double p1 = t_and_data_[2 * i + 1];
        const double p0 = t_and_data_[2 * (i - 1) + 1];
        const double t1mt0 = t1 - t0;
        const double t1mt02 = t1mt0 * t1mt0;
        const double c = -(p1 - p0 - v1 * t1mt0) / t1mt02;
        const double b = v1 - 2 * c * t1;
        const double a = p0 - b * t0 - c * t0 * t0;
        double* a_data = &(new_data[(i - 1) * chunk_size]);
        a_data[0] = t0;
        a_data[1] = a;
        a_data[2] = b;
        a_data[3] = c;

        v1 = b + 2 * c * t0;
    }

    // linear completion
    {
        const double t0 = t_and_data_[2 * (t_size - 1)];
        const double t1 = t_and_data_[0];
        const double p0 = t_and_data_[2 * (t_size - 1) + 1];
        const double p1 = t_and_data_[1];
        const double b = (p1 - p0) / (t1 - t0);
        double* a_data = &(new_data[(t_size - 1) * chunk_size]);
        a_data[0] = t0;
        a_data[1] = p0;
        a_data[2] = b;
        a_data[3] = 0.0;
    }

    delete[] t_and_data_;
    t_and_data_ = new_data;
    new_data = 0;
}

} // namespace GraphicsDatabase
