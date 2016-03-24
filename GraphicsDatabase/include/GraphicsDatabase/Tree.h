#ifndef GRAPHICS_DATABASE__TREE_H_
#define GRAPHICS_DATABASE__TREE_H_
#include <string>

namespace GraphicsDatabase
{

class Database;
class Matrix44;
class Node;
class TreeTemplate;
class Vector3;

class Tree
{
private:
    Node* root_;
    const std::string id_;

public:
    Tree(Database* database, const TreeTemplate* tree_template);
    ~Tree();
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
