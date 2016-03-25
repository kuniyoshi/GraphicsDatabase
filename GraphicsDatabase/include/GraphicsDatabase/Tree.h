#ifndef GRAPHICS_DATABASE__TREE_H_
#define GRAPHICS_DATABASE__TREE_H_
#include <string>
#include <utility>
#include <vector>

using std::pair;
using std::vector;

namespace GraphicsDatabase
{

class Animation;
class Database;
class Matrix44;
class Node;
class TreeTemplate;
class Vector3;

class Tree
{
private:
    unsigned time_;
    // unsigned max_animation_period_; // group time to avoid overflow
    double time_rate_;
    Node* root_;
    const std::string id_;
    vector< pair< Node*, const Animation* > > nodes_animations_pairs_;

public:
    Tree(Database* database, const TreeTemplate* tree_template);
    ~Tree();
    double time_rate() const;
    void time_rate(double new_value);

    void draw(  const Matrix44& perspective_matrix,
                const Vector3& brightness,
                double ambient_brightness,
                const Vector3& light_vector) const;
    void draw_flat_shading( const Matrix44& perspective_matrix,
                            const Vector3& brightness,
                            double ambient_brightness,
                            const Vector3& light_vector) const;
    void update(unsigned delta_time);
};

} // namespace GraphicsDatabase

#endif
