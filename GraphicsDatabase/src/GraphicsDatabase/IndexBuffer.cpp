#include "GraphicsDatabase/IndexBuffer.h"
#include <vector>

namespace GraphicsDatabase
{

IndexBuffer::IndexBuffer(   const std::vector< int >& indexes,
                            const std::vector< double >& uvs)
: indexes_(0), uvs_(0), indexes_size_(0)
{
    assert((indexes.size() % 3) == 0);
    assert((uvs.size() % 6) == 0);
    assert(indexes.size() == uvs.size());

    indexes_size_ = indexes.size();
    indexes_ = new int[indexes_size_];
    uvs_ = new double[indexes_size_];

    std::vector< int >::const_iterator iterator = indexes.begin();

    for (; iterator != indexes.end(); ++iterator)
    {
        indexes_[iterator] = *iterator;
    }

    std::vector< double >::const_iterator uvs_iterator = uvs.begin();

    for (; uvs_iterator != uvs.end(); ++uvs_iterator)
    {
        uvs_[uvs_iterator] = *uvs_iterator;
    }
}

IndexBuffer::~IndexBuffer()
{
    delete[] indexes_;
    delete[] uvs_;
}

size_t IndexBuffer::at(size_t index) const { return indexes_[index]; }

Vector2* IndexBuffer::uv_at(size_t index) const { return uvs_[index]; }

size_t IndexBuffer::size() const { return }

} // namespace GraphicsDatabase
