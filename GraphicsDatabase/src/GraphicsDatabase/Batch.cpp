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

namespace
{

double clamp01(double value)
{
    if (value < 0.0)
    {
        return 0.0;
    }
    else if (value > 1.0)
    {
        return 1.0;
    }

    return value;
}

unsigned calc_color(    const Vector3& brightness,
                        const Vector3& light,
                        const Vector3& normal,
                        double ambient_brightness)
{
    double cosine = light.dot(normal) / normal.length();

    if (cosine < 0.0)
    {
        cosine = 0.0;
    }

    double color_r = brightness.x * cosine + ambient_brightness;
    double color_g = brightness.y * cosine + ambient_brightness;
    double color_b = brightness.z * cosine + ambient_brightness;

    int r = static_cast< int >(255 * color_r + 0.5);
    int g = static_cast< int >(255 * color_g + 0.5);
    int b = static_cast< int >(255 * color_b + 0.5);

    unsigned color = (255 << 24) + (r << 16) + (g << 8) + b;

    return color;
}

} // namespace -

void Batch::draw(   const Matrix44& world_matrix,
                    const Matrix44& perspective_matrix,
                    const Vector3& brightness,
                    double ambient_brightness,
                    const Vector3& light_vector)
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
                world_matrix.multiply(vertexes[i]);
                did_change_[indexes[i]] = true;
            }
        }

        Vector3 p01(*vertexes[1]);
        p01.subtract(*vertexes[0]);
        Vector3 p02(*vertexes[2]);
        p02.subtract(*vertexes[0]);
        p02.cross_product(p01);
        Vector3* normal = &p02;
        unsigned color = calc_color(    brightness,
                                        light_vector,
                                        *normal,
                                        ambient_brightness);

        Vector3 vertexes0(*vertexes[0]);
        Vector3 vertexes1(*vertexes[1]);
        Vector3 vertexes2(*vertexes[2]);

        perspective_matrix.multiply(&vertexes0);
        perspective_matrix.multiply(&vertexes1);
        perspective_matrix.multiply(&vertexes2);

        f.drawTriangle3DH(  &(vertexes0.x),
                            &(vertexes1.x),
                            &(vertexes2.x),
                            &(uv[0]->u),
                            &(uv[1]->u),
                            &(uv[2]->u),
                            color,
                            color,
                            color);
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
