#ifndef GRAPHICS_DATABASE__NODE_H_
#define GRAPHICS_DATABASE__NODE_H_

namespace GraphicsDatabase
{

class Model;

class Node
{
private:
    Model* model_;
    Node* children_;
    int children_size_;

public:
    Node();
    Node(int children_size);
    ~Node();
    void set_model(const Model*);
    void set_child(const Node* child, int index);
    void draw(  const Matrix44& perspective_matrix,
            const Vector3& brightness,
            double ambient_brightness,
            const Vector3& light_vector) const;
};

} // namespace GraphicsDatabase

#endif
