#include "GraphicsDatabase/Tree.h"
#include <sstream>
#include <string>
#include "GraphicsDatabase/Database.h"
#include "GraphicsDatabase/Node.h"
#include "GraphicsDatabase/NodeTemplate.h"
#include "GraphicsDatabase/TreeTemplate.h"

namespace GraphicsDatabase
{

namespace
{

void append_children(   Database* database,
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
        append_children(database, child, *child_template);
    }
}

} // namespace -

Tree::Tree(Database* database, const TreeTemplate* tree_template)
:   root_(0), id_(*tree_template->id())
{
    const NodeTemplate* node_template = tree_template->root();
    root_ = new Node(id_);
    append_children(database, root_, *node_template);
}

Tree::~Tree()
{
    delete root_;
    root_ = 0;
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

} // namespace GraphicsDatabase
