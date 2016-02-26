#include "GraphicsDatabase/IndexBuffer.h"
#include <cassert>
#include <vector>
#include "GraphicsDatabase/Vector2.h"

namespace GraphicsDatabase
{

IndexBuffer::IndexBuffer(   const std::vector< int >& indexes,
                            const std::vector< double >& uvs)
: indexes_(0), uvs_(0), indexes_size_(0)
{
    assert((indexes.size() % 3) == 0);
    assert((uvs.size() % 6) == 0);
    assert(2 * indexes.size() == uvs.size());

    indexes_size_ = indexes.size();
    indexes_ = new int[indexes_size_];
    uvs_ = new Vector2[indexes_size_];

    for (size_t i = 0; i < indexes_size_; ++i)
    {
        indexes_[i] = indexes[i];
    }

    for (size_t i = 0; i < uvs.size(); i += 2)
    {
        uvs_[i / 2].set(uvs[i], uvs[i + 1]);
    }
}

IndexBuffer::~IndexBuffer()
{
    delete[] indexes_;
    delete[] uvs_;
}

const int* IndexBuffer::indexes() const { return indexes_; }

size_t IndexBuffer::at(size_t index) const { return indexes_[index]; }

Vector2* IndexBuffer::uv_at(size_t index) const { return &uvs_[index]; }

size_t IndexBuffer::size() const { return indexes_size_; }

} // namespace GraphicsDatabase
