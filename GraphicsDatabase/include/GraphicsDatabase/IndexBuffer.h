#ifndef GRAPHICS_DATABASE__INDEX_BUFFER_H_
#define GRAPHICS_DATABASE__INDEX_BUFFER_H_
#include <vector>

namespace GraphicsDatabase
{

class Vector2;

class IndexBuffer
{
private:
    Vector2* dummy_;
    Vector2* uvs_;
    bool has_uvs_;
    int* indexes_;
    size_t indexes_size_;

public:
    IndexBuffer(    const std::vector< int >& indexes,
                    const std::vector< double >& uvs);
    ~IndexBuffer();
    const int* indexes() const;
    size_t at(size_t index) const;
    Vector2* uv_at(size_t index) const;
    size_t size() const;
};

} // namespace GraphicsDatabase

#endif
