#ifndef GRAPHICS_DATABASE__DATABASE_H_
#define GRAPHICS_DATABASE__DATABASE_H_
#include <map>

namespace GameLib { class Texture; }

namespace GraphicsDatabase
{

class Animation;
class Batch;
class IndexBuffer;
class Model;
class Tree;
class TreeTemplate;
class VertexBuffer;

class Database
{
private:
    std::map< const std::string, Animation* > animation_;
    std::map< const std::string, Batch* > batch_;
    std::map< const std::string, GameLib::Texture* > texture_;
    std::map< const std::string, IndexBuffer* > index_buffer_;
    std::map< const std::string, Model* > model_;
    std::map< const std::string, VertexBuffer* > vertex_buffer_;
    std::map< const std::string, Tree* > tree_;
    std::map< const std::string, TreeTemplate* > tree_template_;

public:
    Database(const char* filename);
    ~Database();
    void create(const std::string& model_id, const std::string& batch_id);
    void create_tree(   const std::string& tree_id,
                        const std::string& template_id);
    Model* find(const std::string& model_id) const;
    Animation* find_animation(const std::string& animation_id) const;
    Tree* find_tree(const std::string& tree_id) const;
};

} // namespace GraphicsDatabase

#endif
