#ifndef GRAPHICS_DATABASE__VERTEX_BUFFER_H_
#define GRAPHICS_DATABASE__VERTEX_BUFFER_H_
#include <vector>

namespace GraphicsDatabase
{

class Vector3;

class VertexBuffer
{
private:
    Vector3* vertexes_;
    size_t vertexes_size_;

public:
    VertexBuffer();
    VertexBuffer(const std::vector< double >& vertexes);
    ~VertexBuffer();
    Vector3* at(size_t index) const;
    // void set_size(size_t size);
    void copy_from(const VertexBuffer& base);
    void set_vertexes(const std::vector< double >& vertexes);
    size_t size() const;
};

} // namespace GraphicsDatabase

#endif
