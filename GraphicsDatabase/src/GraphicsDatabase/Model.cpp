#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/Batch.h"

namespace GraphicsDatabase
{

Model::Model() : batch_(0) {}

Model::Model(const Batch* batch) : batch_(batch) {}

Model::~Model() {}

} // namespace GraphicsDatabase
