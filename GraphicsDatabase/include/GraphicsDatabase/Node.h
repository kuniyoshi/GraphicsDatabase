#ifndef GRAPHICS_DATABASE__NODE_H_
#define GRAPHICS_DATABASE__NODE_H_
#include <string>

namespace GraphicsDatabase
{

class Animation;
class Matrix44;
class Model;
class NodeTemplate;
class Vector3;

class Node
{
private:
    const std::string id_;
    Model* model_;
    Node** children_;
    unsigned children_size_;
    bool does_model_in_database_;

public:
    Node(const std::string& id);
    ~Node();
    const std::string* id() const;
    Model* model();
    void model(Model* new_value);
    size_t children_size() const;
    void children_size(int new_value);
    void set_child(Node* child, int index);
    void setup_model(const NodeTemplate& note_template);
    void reserve_children();

    void draw(  const Matrix44& perspective_matrix,
                const Vector3& brightness,
                double ambient_brightness,
                const Vector3& light_vector) const;
    void draw_flat_shading( const Matrix44& perspective_matrix,
                            const Vector3& brightness,
                            double ambient_brightness,
                            const Vector3& light_vector) const;
    void update(const double time, const Animation& animation);
};

} // namespace GraphicsDatabase

#endif
