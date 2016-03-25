#ifndef GRAPHICS_DATABASE__NODE_TEMPLATE_H_
#define GRAPHICS_DATABASE__NODE_TEMPLATE_H_
#include <string>
#include <vector>

namespace GraphicsDatabase
{

class NodeTemplate
{
private:
    std::string id_;
    std::string model_id_;
    std::string animation_id_;
    NodeTemplate** children_;
    size_t children_size_;
    double scale_;
    double angle_[3];
    double position_[3];

public:  
    NodeTemplate(const std::string& id);
    ~NodeTemplate();
    const std::string* id() const;
    const std::string* model_id() const;
    void model_id(const std::string& new_value);
    const std::string* animation_id() const;
    void animation_id(const std::string& new_value);
    size_t children_size() const;
    void children_size(int new_value);
    double scale() const;
    void scale(double new_value);
    const double* angle() const;
    void angle(const std::vector< double >& angle);
    const double* position() const;
    void position(const std::vector< double >& position);

    const NodeTemplate* child(int index) const;
    bool has_animation() const;
    void reserve_children();
    void set_child(NodeTemplate* child, int index);
};

} // namespace GraphicsDatabase

#endif
