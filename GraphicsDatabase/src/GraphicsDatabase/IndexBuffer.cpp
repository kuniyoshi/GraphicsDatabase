#include "GraphicsDatabase/IndexBuffer.h"
#include <cassert>
#include <vector>
#include "GraphicsDatabase/Vector2.h"

namespace GraphicsDatabase
{

IndexBuffer::IndexBuffer(   const std::vector< int >& indexes,
                            const std::vector< double >& uvs)
: dummy_(0), uvs_(0), has_uvs_(true), indexes_(0), indexes_size_(0)
{
    assert((indexes.size() % 3) == 0);
    assert((uvs.size() % 6) == 0);
    assert(!uvs.size() || (2 * indexes.size() == uvs.size()));

    indexes_size_ = indexes.size();
    indexes_ = new int[indexes_size_];

    for (size_t i = 0; i < indexes_size_; ++i)
    {
        indexes_[i] = indexes[i];
    }

    if (uvs.size())
    {
        has_uvs_ = true;
        uvs_ = new Vector2[indexes_size_];
        dummy_ = 0;

        for (size_t i = 0; i < uvs.size(); i += 2)
        {
            uvs_[i / 2].set(uvs[i], uvs[i + 1]);
        }
    }
    else
    {
        has_uvs_ = false;
        dummy_ = new Vector2[3];
        dummy_[0].set(0.0, 0.0);
        dummy_[1].set(0.0, 0.0);
        dummy_[2].set(0.0, 0.0);
        uvs_ = 0;
    }
}

IndexBuffer::~IndexBuffer()
{
    delete[] indexes_;
    indexes_ = 0;

    if (uvs_)
    {
        delete[] uvs_;
        uvs_ = 0;
    }

    if (dummy_)
    {
        delete[] dummy_;
        dummy_ = 0;
    }
}

const int* IndexBuffer::indexes() const { return indexes_; }

size_t IndexBuffer::at(size_t index) const { return indexes_[index]; }

Vector2* IndexBuffer::uv_at(size_t index) const
{
    return has_uvs_ ? &uvs_[index] : dummy_;
}

size_t IndexBuffer::size() const { return indexes_size_; }

} // namespace GraphicsDatabase
