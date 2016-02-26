#include "GraphicsDatabase/Batch.h"
#include "GameLib/Framework.h"
#include "GraphicsDatabase/IndexBuffer.h"
#include "GraphicsDatabase/Matrix44.h"
#include "GraphicsDatabase/Vector2.h"
#include "GraphicsDatabase/Vector3.h"
#include "GraphicsDatabase/VertexBuffer.h"

namespace GraphicsDatabase
{

Batch::Batch(   VertexBuffer* vertex_buffer,
                IndexBuffer* index_buffer,
                GameLib::Texture* texture)
:   master_vertex_buffer_(vertex_buffer),
    vertex_buffer_(),
    index_buffer_(index_buffer),
    texture_(texture)
{
    vertex_buffer_.copy_from(*master_vertex_buffer_);
    did_change_ = new bool[master_vertex_buffer_->size()];
}

Batch::~Batch()
{
    delete[] did_change_;
}

void Batch::draw(const Matrix44& wvp)
{
    GameLib::Framework f = GameLib::Framework::instance();
    f.enableDepthTest(true);
    f.enableDepthWrite(true);
    f.setTexture(texture_);

    vertex_buffer_.copy_from(*master_vertex_buffer_);

    for (size_t i = 0; i < master_vertex_buffer_->size(); ++i)
    {
        did_change_[i] = false;
    }

    for (size_t i = 0; i < index_buffer_->size(); i = i + 3)
    {
        const size_t indexes[3] = { index_buffer_->at(i),
                                    index_buffer_->at(i + 1),
                                    index_buffer_->at(i + 2)};
        Vector3* vertexes[3];
        vertexes[0] = vertex_buffer_.at(indexes[0]);
        vertexes[1] = vertex_buffer_.at(indexes[1]);
        vertexes[2] = vertex_buffer_.at(indexes[2]);

        Vector2* uv[3];
        uv[0] = index_buffer_->uv_at(i);
        uv[1] = index_buffer_->uv_at(i + 1);
        uv[2] = index_buffer_->uv_at(i + 2);

        for (int i = 0; i < 3; ++i)
        {
            if (!did_change_[indexes[i]])
            {
                wvp.multiply(vertexes[i]);
                did_change_[indexes[i]] = true;
            }
        }

        f.drawTriangle3DH(  &(vertexes[0]->x),
                            &(vertexes[1]->x),
                            &(vertexes[2]->x),
                            &(uv[0]->u),
                            &(uv[1]->u),
                            &(uv[2]->u));
    }
}

const Vector3* Batch::vertexes() const
{
    return master_vertex_buffer_->vertexes();
}

size_t Batch::vertexes_size() const
{
    return master_vertex_buffer_->size();
}

const int* Batch::indexes() const
{
    return index_buffer_->indexes();
}

size_t Batch::indexes_size() const
{
    return index_buffer_->size();
}

} // namespace GraphicsDatabase
