#include "GraphicsDatabase/Tree.h"
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "GraphicsDatabase/Animation.h"
#include "GraphicsDatabase/Database.h"
#include "GraphicsDatabase/Node.h"
#include "GraphicsDatabase/Model.h"
#include "GraphicsDatabase/NodeTemplate.h"
#include "GraphicsDatabase/TreeTemplate.h"

using std::pair;
using std::vector;

namespace GraphicsDatabase
{

typedef std::pair< Node*, const Animation* > NodeAnimation;

namespace
{

void append_children(   Database* database,
                        vector< NodeAnimation >* nodes_animations_pairs,
                        Node* parent,
                        const NodeTemplate& parent_template)
{
    std::string model_id = *parent_template.model_id();

    if (!!model_id.size())
    {
        database->create(*parent->id(), model_id);
        parent->model(database->find(*parent->id()));
    }

    parent->setup_model(parent_template);

    const Animation* animation = 0;

    if (parent_template.has_animation())
    {
        animation = database->find_animation(*parent_template.animation_id());
    }

    nodes_animations_pairs->push_back(NodeAnimation(parent, animation));

    size_t children_size = parent_template.children_size();

    if (!children_size)
    {
        return;
    }

    parent->children_size(children_size);
    parent->reserve_children();

    std::ostringstream oss;

    for (size_t i = 0; i < children_size; ++i)
    {
        oss.str("");
        oss << parent->id() << "." << i;
        Node* child = new Node(oss.str());
        parent->set_child(child, i);

        const NodeTemplate* child_template = parent_template.child(i);
        append_children(    database,
                            nodes_animations_pairs,
                            child,
                            *child_template);
    }
}

} // namespace -

Tree::Tree(Database* database, const TreeTemplate* tree_template)
:   time_(0), time_rate_(1.0), // max_animation_period_(0),
    root_(0), id_(*tree_template->id()),
    nodes_animations_pairs_()
{
    const NodeTemplate* node_template = tree_template->root();
    root_ = new Node(id_);
    nodes_animations_pairs_.push_back(NodeAnimation(root_, 0));
    append_children(    database,
                        &nodes_animations_pairs_,
                        root_,
                        *node_template);
}

Tree::~Tree()
{
    delete root_;
    root_ = 0;
}

void Tree::time_rate(double new_value) { time_rate_ = new_value; }

double Tree::time_rate() const { return time_rate_; }

namespace
{

const Model* get_const_balance_model(const Node& root)
{
    return root.model();
}

Model* get_balance_model(Node* root)
{
    return root->model();
}

} // namespace -

const Model* Tree::root() const { return get_const_balance_model(*root_); }

void Tree::angle(const Vector3& new_value)
{
    get_balance_model(root_)->angle(new_value);
}

const Vector3* Tree::angle() const
{
    return get_const_balance_model(*root_)->angle();
}

void Tree::balance(const Vector3& new_value)
{
    get_balance_model(root_)->position(new_value);
}

const Vector3* Tree::balance() const
{
    return get_const_balance_model(*root_)->position();
}

void Tree::draw(    const Matrix44& perspective_matrix,
                    const Vector3& brightness,
                    double ambient_brightness,
                    const Vector3& light_vector) const
{
    root_->draw(    perspective_matrix,
                    brightness,
                    ambient_brightness,
                    light_vector);
}

void Tree::draw_flat_shading(   const Matrix44& perspective_matrix,
                                const Vector3& brightness,
                                double ambient_brightness,
                                const Vector3& light_vector) const
{
    root_->draw_flat_shading(   perspective_matrix,
                                brightness,
                                ambient_brightness,
                                light_vector);
}

void Tree::update(unsigned delta_time)
{
    time_ += static_cast< unsigned >(static_cast< double >(delta_time) * time_rate_);
    double now = static_cast< double >(time_) / 1000.0; // NOTE group when overflow

    vector< NodeAnimation >::iterator it = nodes_animations_pairs_.begin();

    for (; it != nodes_animations_pairs_.end(); ++it)
    {
        if (!it->second)
        {
            continue;
        }

        it->first->update(now, *it->second);
    }
}

} // namespace GraphicsDatabase
