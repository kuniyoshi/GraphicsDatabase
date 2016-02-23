#ifndef GRAPHICS_DATABASE__BATCH_H_
#define GRAPHICS_DATABASE__BATCH_H_
#include "GraphicsDatabase/VertexBuffer.h"

namespace GameLib { class Texture; }

namespace GraphicsDatabase
{

class Camera;
class IndexBuffer;
class Matrix44;
class Vector3;
class VertexBuffer;

class Batch
{
private:
    VertexBuffer* master_vertex_buffer_;
    VertexBuffer vertex_buffer_;
    IndexBuffer* index_buffer_;
    GameLib::Texture* texture_;
    bool* did_change_;

public:
    Batch(  VertexBuffer* vertex_buffer,
            IndexBuffer* index_buffer,
            GameLib::Texture* texture);
    ~Batch();
    void draw(const Matrix44& wvp_matrix);
    const Vector3* vertexes() const;
    size_t vertexes_size() const;
};

} // namespace GraphicsDatabase

#endif
