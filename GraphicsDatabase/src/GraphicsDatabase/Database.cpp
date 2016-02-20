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
        oss.str("models.");
        oss << i;

        std::string base_key = oss.str();

        oss << ".id";
        std::string id = data.get_at(oss.str());

        oss.str(base_key);
        oss << ".image";
        std::string image_name = data.get_at(oss.str());

        GameLib::Texture* texture;
        f.createTexture(&texture, image_name.c_str());

        oss.str(base_key);
        oss << ".vertexes";

        std::vector< double > vertexes;
        data.copy_expanded_to_vector_at(&vertexes, oss.str());

        VertexBuffer* vertex_buffer = new VertexBuffer(vertexes);
        typedef std::pair< const char*, VertexBuffer* > IdVertexBuffer;
        vertex_buffer_.insert(IdVertexBuffer(id.c_str(), vertex_buffer));

        oss.str(base_key);
        oss << ".indexes";

        std::vector< int > indexes;
        data.copy_expanded_to_vector_at(&indexes, oss.str());

        oss.str(base_key);
        oss << ".uvs";

        std::vector< double > uvs;
        data.copy_2expanded_to_vector_at(&uvs, oss.str());

        IndexBuffer* index_buffer = new IndexBuffer(indexes, uvs);
        typedef std::pair< const char*, IndexBuffer* > IdIndexBuffer;
        index_buffer_.insert(IdIndexBuffer(id.c_str(), index_buffer));

        Batch* batch = new Batch(vertex_buffer, index_buffer, texture);
        typedef std::pair< const char*, Batch* > IdBatch;
        batch_.insert(IdBatch(id.c_str(), batch));
    }
}

Database::~Database()
{
    std::map< const char*, Model* >::iterator model_iterator = model_.begin();

    for (; model_iterator != model_.end(); ++model_iterator)
    {
        delete model_iterator->second;
    }

    std::map< const char*, Batch* >::iterator batch_iterator
    = batch_.begin();

    for (; batch_iterator != batch_.end(); ++batch_iterator)
    {
        delete batch_iterator->second;
    }

    std::map< const char*, IndexBuffer* >::iterator ib_it
    = index_buffer_.begin();

    for (; ib_it != index_buffer_.end(); ++ib_it)
    {
        delete ib_it->second;
    }

    std::map< const char*, VertexBuffer* >::iterator vb_it
    = vertex_buffer_.begin();

    for (; vb_it != vertex_buffer_.end(); ++vb_it)
    {
        delete vb_it->second;
    }
}

void Database::create(const char* model_id, const char* batch_id)
{
    Batch* batch = batch_.at(batch_id);

    std::map< const char*, Model* >::iterator model_iterator
    = model_.find(model_id);
    assert(model_iterator == model_.end());

    Model* model = new Model(batch);
    model_.insert(std::pair< const char*, Model* >(model_id, model));
}

Model* Database::find(const char* model_id) const
{
    return model_.at(model_id);
}

} // namespace GraphicsDatabase
