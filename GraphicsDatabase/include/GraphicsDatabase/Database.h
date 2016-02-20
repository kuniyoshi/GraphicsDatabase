#ifndef GRAPHICS_DATABASE__DATABASE_H_
#define GRAPHICS_DATABASE__DATABASE_H_
#include <map>

namespace GraphicsDatabase
{

class Batch;
class IndexBuffer;
class Model;
class VertexBuffer;

class Database
{
private:
    std::map< const char*, VertexBuffer* > vertex_buffer_;
    std::map< const char*, IndexBuffer* > index_buffer_;
    std::map< const char*, Batch* > batch_;
    std::map< const char*, Model* > model_;

public:
    Database(const char* filename);
    ~Database();
    void create(const char* model_id, const char* batch_id);
    Model* find(const char* model_id) const;
};

} // namespace GraphicsDatabase

#endif
