#include "GraphicsDatabase/Database.h"
#include <cassert>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "PseudoJson/Decoder.h"
#include "PseudoJson/Data.h"
#include "GameLib/Framework.h"
#include "GraphicsDatabase/Batch.h"
#include "GraphicsDatabase/IndexBuffer.h"
#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/VertexBuffer.h"

namespace GraphicsDatabase
{

Database::Database(const char* filename)
: batch_(), model_()
{
    GameLib::Framework f = ::GameLib::Framework::instance();
    PseudoJson::Decoder decoder(filename);
    decoder.decode();
    const PseudoJson::Data data(decoder.data());

    size_t model_size = data.size_of("models");

    std::ostringstream oss;

    for (size_t i = 0; i < model_size; ++i)
    {
        oss.str("");
        oss << "models." << i;
        const std::string base_key = oss.str();
        const std::string id = data.get_at(base_key + ".id");

        const std::string image_name = data.get_at(base_key + ".image");

        GameLib::Texture* texture = 0;
        f.createTexture(&texture, image_name.c_str());

        typedef std::pair< const std::string, GameLib::Texture* > IdTexture;
        texture_.insert(IdTexture(id, texture));

        std::vector< double > vertexes;
        data.copy_expanded_to_vector_at(&vertexes, base_key + ".vertexes");

        VertexBuffer* vertex_buffer = new VertexBuffer(vertexes);
        typedef std::pair< const std::string, VertexBuffer* > IdVertexBuffer;
        vertex_buffer_.insert(IdVertexBuffer(id, vertex_buffer));

        std::vector< int > indexes;
        data.copy_expanded_to_vector_at(&indexes, base_key + ".indexes");

        std::vector< double > uvs;
        data.copy_2expanded_to_vector_at(&uvs, base_key + ".uvs");

        IndexBuffer* index_buffer = new IndexBuffer(indexes, uvs);
        typedef std::pair< const std::string, IndexBuffer* > IdIndexBuffer;
        index_buffer_.insert(IdIndexBuffer(id, index_buffer));

        Batch* batch = new Batch(vertex_buffer, index_buffer, texture);
        typedef std::pair< const std::string, Batch* > IdBatch;
        batch_.insert(IdBatch(id, batch));
    }
}

Database::~Database()
{
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

Model* Database::find(const std::string& model_id) const
{
    return model_.at(model_id);
}

} // namespace GraphicsDatabase
