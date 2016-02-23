#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/Batch.h"
#include "GraphicsDatabase/Matrix44.h"
#include "GraphicsDatabase/Vector3.h"

namespace GraphicsDatabase
{

Model::Model()
:   batch_(0),
    position_()
{}

Model::Model(Batch* batch) : batch_(batch) {}

Model::~Model() {}

const Vector3& Model::position() const { return position_; }

void Model::position(const Vector3& new_value) { position_ = new_value; }

void Model::draw(const Matrix44& perspective_matrix)
{
    Matrix44 wvp;
    wvp.translate(position_);
    wvp.dot(perspective_matrix);
    batch_->draw(wvp);
}

const Vector3* Model::vertexes() const { return batch_->vertexes(); }

size_t Model::vertexes_size() const { return batch_->vertexes_size(); }

} // namespace GraphicsDatabase
