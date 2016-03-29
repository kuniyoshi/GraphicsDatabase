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

    assert(false);
}

size_t get_chunk_size(const TheCompletion::Method method)
{
    switch (method)
    {
        case TheCompletion::MethodLastOne: return 2; break;
        case TheCompletion::MethodLinear: return 2; break;
    }

    assert(false);
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

} // namespace GraphicsDatabase
