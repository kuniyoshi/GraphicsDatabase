#ifndef GRAPHICS_DATABASE__MODEL_H_
#define GRAPHICS_DATABASE__MODEL_H_

namespace GraphicsDatabase
{

class Batch;

class Model
{
private:
    const Batch* batch_;

public:
    Model();
    Model(const Batch* batch);
    ~Model();
};

} // namespace GraphicsDatabase

#endif
