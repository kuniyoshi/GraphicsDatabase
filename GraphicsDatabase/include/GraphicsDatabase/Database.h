#ifndef GRAPHICS_DATABASE__DATABASE_H_
#define GRAPHICS_DATABASE__DATABASE_H_
#include <map>

namespace GameLib { class Texture; }

namespace GraphicsDatabase
{

class Batch;
class IndexBuffer;
class Model;
class VertexBuffer;

class Database
{
private:
    std::map< const std::string, VertexBuffer* > vertex_buffer_;
    std::map< const std::string, IndexBuffer* > index_buffer_;
    std::map< const std::string, Batch* > batch_;
    std::map< const std::string, Model* > model_;
    std::map< const std::string, GameLib::Texture* > texture_;

public:
    Database(const char* filename);
    ~Database();
    void create(const std::string& model_id);
    void create(const std::string& model_id, const std::string& batch_id);
    Model* find(const std::string& model_id) const;
};

} // namespace GraphicsDatabase

#endif
