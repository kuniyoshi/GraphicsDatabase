#include "GraphicsDatabase/VertexBuffer.h"
#include <cassert>
#include <vector>
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

VertexBuffer::VertexBuffer() : vertexes_(0), vertexes_size_(0) {}

VertexBuffer::VertexBuffer(const std::vector< double >& vertexes)
: vertexes_(0), vertexes_size_(0)
{
    set_vertexes(vertexes);
}

VertexBuffer::~VertexBuffer()
{
    delete[] vertexes_;
}

Vector3* VertexBuffer::at(size_t index) const
{
    assert(index >= 0 && index < vertexes_size_);
    return &vertexes_[index];
}

void VertexBuffer::copy_from(const VertexBuffer& base)
{
    if (base.vertexes_size_ != vertexes_size_)
    {
        assert(!vertexes_);
        assert(vertexes_size_ == 0);

        vertexes_size_ = base.vertexes_size_;
        vertexes_ = new Vector3[vertexes_size_];
    }

    for (size_t i = 0; i < base.vertexes_size_; ++i)
    {
        vertexes_[i] = base.vertexes_[i];
    }
}

// void VertexBuffer::set_size(size_t size)
// {
//     assert(!vertexes_);
//     vertexes_ = new Vector3[size];
// }

void VertexBuffer::set_vertexes(const std::vector< double >& vertexes)
{
    assert((vertexes.size() % 3) == 0);
    assert(!vertexes_);
    vertexes_size_ = vertexes.size();
    vertexes_ = new Vector3[vertexes_size_];

    for (size_t i = 0; i < vertexes.size(); i = i + 3)
    {
        vertexes_[i].set(vertexes[i], vertexes[i + 1], vertexes[i + 2]);
    }
}

size_t VertexBuffer::size() const { return vertexes_size_; }

} // namespace GraphicsDatabase
