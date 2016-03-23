#ifndef GRAPHICS_DATABASE__NODE_H_
#define GRAPHICS_DATABASE__NODE_H_

namespace GraphicsDatabase
{

class Matrix44;
class Model;
class Vector3;

class Node
{
private:
    const Model* model_;
    Node** children_;
    int children_size_;

public:
    Node();
    Node(int children_size);
    ~Node();
    void init_children(int children_size);
    void set_model(const Model*);
    void set_child(Node* child, int index);

    void draw(  const Matrix44& perspective_matrix,
                const Vector3& brightness,
                double ambient_brightness,
                const Vector3& light_vector) const;
    void draw_flat_shading( const Matrix44& perspective_matrix,
                            const Vector3& brightness,
                            double ambient_brightness,
                            const Vector3& light_vector) const;
};

} // namespace GraphicsDatabase

#endif
