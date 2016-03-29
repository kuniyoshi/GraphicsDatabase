#include "GraphicsDatabase/Database.h"
#include <cassert>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "PseudoJson/Decoder.h"
#include "PseudoJson/Data.h"
#include "GameLib/Framework.h"
#include "GraphicsDatabase/Animation.h"
#include "GraphicsDatabase/Batch.h"
#include "GraphicsDatabase/IndexBuffer.h"
#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/NodeTemplate.h"
#include "GraphicsDatabase/Tree.h"
#include "GraphicsDatabase/TreeTemplate.h"
#include "GraphicsDatabase/VertexBuffer.h"

namespace GraphicsDatabase
{

namespace
{

void append_children(   NodeTemplate* parent,
                        const PseudoJson::Data& data,
                        const std::string& key)
{
    if (data.does_exist(key + ".model"))
    {
        parent->model_id(data.get_at(key + ".model"));
    }

    if (data.does_exist(key + ".scale"))
    {
        parent->scale(data.get_double_at(key + ".scale"));
    }

    if (data.does_exist(key + ".angle"))
    {
        std::vector< double > angle;
        data.copy_to_vector_at(&angle, key + ".angle");
        parent->angle(angle);
    }

    if (data.does_exist(key + ".position"))
    {
        std::vector< double > position;
        data.copy_to_vector_at(&position, key + ".position");
        parent->position(position);
    }

    if (data.does_exist(key + ".animation"))
    {
        parent->animation_id(data.get_at(key + ".animation"));
    }

    if (!data.does_exist(key + ".children"))
    {
        return;
    }

    size_t size = data.size_of(key + ".children");

    if (!size)
    {
        return;
    }

    const size_t children_size = data.size_of(key + ".children");
    parent->children_size(children_size);
    parent->reserve_children();

    std::ostringstream oss;

    for (size_t i = 0; i < size; ++i)
    {
        oss.str("");
        oss << key << ".children." << i;
        const std::string base_key = oss.str();
        const std::string child_key = base_key + data.get_at(base_key + ".id");

        NodeTemplate* child = new NodeTemplate(child_key);
        parent->set_child(child, i);

        append_children(child, data, base_key);
    }
}

} // namespace -

Database::Database(const char* filename)
:   vertex_buffer_(), index_buffer_(),
    batch_(), model_(), texture_()
{
    GameLib::Framework f = ::GameLib::Framework::instance();
    PseudoJson::Decoder decoder(filename);
    decoder.decode();
    const PseudoJson::Data data(decoder.data());

    std::ostringstream oss;

    // read models
    {
        const size_t model_size = data.size_of("models");

        for (size_t i = 0; i < model_size; ++i)
        {
            oss.str("");
            oss << "models." << i;
            const std::string base_key = oss.str();
            const std::string id = data.get_at(base_key + ".id");

            const std::string image_key = base_key + ".image";
            GameLib::Texture* texture = 0;

            if (data.does_exist(image_key))
            {
                const std::string image_name = data.get_at(image_key);
                f.createTexture(&texture, image_name.c_str());
                typedef std::pair< const std::string, GameLib::Texture* > IdTexture;
                texture_.insert(IdTexture(id, texture));
            }

            std::vector< double > vertexes;
            data.copy_expanded_to_vector_at(&vertexes, base_key + ".vertexes");

            VertexBuffer* vertex_buffer = new VertexBuffer(vertexes);
            typedef std::pair< const std::string, VertexBuffer* > IdVertexBuffer;
            vertex_buffer_.insert(IdVertexBuffer(id, vertex_buffer));

            std::vector< int > indexes;
            data.copy_expanded_to_vector_at(&indexes, base_key + ".indexes");

            const std::string uvs_key = base_key + ".uvs";

            assert(data.does_exist(image_key) == data.does_exist(uvs_key));

            std::vector< double > uvs;

            if (data.does_exist(uvs_key))
            {
                data.copy_2expanded_to_vector_at(&uvs, uvs_key);
            }

            IndexBuffer* index_buffer = new IndexBuffer(indexes, uvs);
            typedef std::pair< const std::string, IndexBuffer* > IdIndexBuffer;
            index_buffer_.insert(IdIndexBuffer(id, index_buffer));

            Batch* batch = new Batch(vertex_buffer, index_buffer, texture);
            typedef std::pair< const std::string, Batch* > IdBatch;
            batch_.insert(IdBatch(id, batch));
        }
    }

    // read animations
    {
        const size_t animations_size = data.size_of("animations");

        for (size_t i = 0; i < animations_size; ++i)
        {
            oss.str("");
            oss << "animations." << i;
            const std::string base_key = oss.str();
            const std::string id = data.get_at(base_key + ".id");

            Animation* animation = new Animation(id);

            if (data.does_exist(base_key + ".angle"))
            {
                const std::string key = base_key + ".angle";
                const char axises[3] = { 'x', 'y', 'z' };

                for (int i = 0; i < 3; ++i)
                {
                    const std::string axis_key = key + "." + axises[i];

                    if (data.does_exist(axis_key))
                    {
                        const std::string method_key = axis_key + ".completion";
                        const std::string period_key = axis_key + ".period";
                        const std::string points_key = axis_key + ".points";
                        const std::string completion_id = data.get_at(method_key);
                        const double period = data.get_double_at(period_key);
                        std::vector< double > axis;
                        data.copy_expanded_to_vector_at(&axis, points_key);
                        animation->angle_completion(    completion_id,
                                                        axises[i],
                                                        axis,
                                                        period);
                    }
                }
            }

            if (data.does_exist(base_key + ".position"))
            {
                // todo
            }

            if (data.does_exist(base_key + ".scale"))
            {
                // todo
            }

            typedef std::pair< const std::string, Animation* > IdAnimation;
            animation_.insert(IdAnimation(id, animation));
        }
    }

    // read trees
    {
        const size_t trees_size = data.size_of("trees");

        for (size_t i = 0; i < trees_size; ++i)
        {
            oss.str("");
            oss << "trees." << i;
            const std::string base_key = oss.str();
            const std::string id = data.get_at(base_key + ".id");

            TreeTemplate* tree_template = new TreeTemplate(id);
            NodeTemplate* root = new NodeTemplate(id);
            tree_template->root(root);
            append_children(root, data, base_key);

            typedef std::pair< const std::string, TreeTemplate* > IdTreeTemplate;
            tree_template_.insert(IdTreeTemplate(id, tree_template));
        }
    }
}

Database::~Database()
{
    std::map< const std::string, Animation* >::iterator animation_it
    = animation_.begin();

    for (; animation_it != animation_.end(); ++animation_it)
    {
        SAFE_DELETE(animation_it->second);
    }

    std::map< const std::string, Tree* >::iterator tree_it
    = tree_.begin();

    for (; tree_it != tree_.end(); ++tree_it)
    {
        SAFE_DELETE(tree_it->second);
    }

    std::map< const std::string, TreeTemplate* >::iterator tree_template_it
    = tree_template_.begin();

    for (; tree_template_it != tree_template_.end(); ++tree_template_it)
    {
        SAFE_DELETE(tree_template_it->second);
    }

    std::map< const std::string, Model* >::iterator model_iterator
    = model_.begin();

    for (; model_iterator != model_.end(); ++model_iterator)
    {
        SAFE_DELETE(model_iterator->second);
    }

    std::map< const std::string, Batch* >::iterator batch_iterator
    = batch_.begin();

    for (; batch_iterator != batch_.end(); ++batch_iterator)
    {
        SAFE_DELETE(batch_iterator->second);
    }

    std::map< const std::string, IndexBuffer* >::iterator ib_it
    = index_buffer_.begin();

    for (; ib_it != index_buffer_.end(); ++ib_it)
    {
        SAFE_DELETE(ib_it->second);
    }

    std::map< const std::string, VertexBuffer* >::iterator vb_it
    = vertex_buffer_.begin();

    for (; vb_it != vertex_buffer_.end(); ++vb_it)
    {
        SAFE_DELETE(vb_it->second);
    }

    GameLib::Framework f = GameLib::Framework::instance();

    std::map< const std::string, GameLib::Texture* >::iterator tx_it
    = texture_.begin();

    for (; tx_it != texture_.end(); ++tx_it)
    {
        f.destroyTexture(&(tx_it->second));
    }
}

void Database::create(const std::string& model_id, const std::string& batch_id)
{
    Batch* batch = batch_.at(batch_id);

    std::map< const std::string, Model* >::iterator model_iterator
    = model_.find(model_id);
    assert(model_iterator == model_.end());

    Model* model = new Model(batch);
    model_.insert(std::pair< const std::string, Model* >(model_id, model));
}

void Database::create_tree( const std::string& tree_id,
                            const std::string& template_id)
{
    std::map< const std::string, Tree* >::iterator tree_it
    = tree_.find(tree_id);
    assert(tree_it == tree_.end());

    std::map< const std::string, TreeTemplate* >::iterator template_it
    = tree_template_.find(template_id);
    assert(template_it != tree_template_.end());

    Tree* tree = new Tree(this, template_it->second);
    tree_.insert(std::pair< const std::string, Tree* >(tree_id, tree));
}

Model* Database::find(const std::string& model_id) const
{
    return model_.at(model_id);
}

Animation* Database::find_animation(const std::string& animation_id) const
{
    return animation_.at(animation_id);
}

Tree* Database::find_tree(const std::string& tree_id) const
{
    return tree_.at(tree_id);
}

} // namespace GraphicsDatabase
