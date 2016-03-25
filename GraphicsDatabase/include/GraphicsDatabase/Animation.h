#ifndef GRAPHICS_DATABASE__ANIMATION_H_
#define GRAPHICS_DATABASE__ANIMATION_H_
#include <string>
#include <vector>
#include "GraphicsDatabase/TheCompletion.h"

namespace GraphicsDatabase
{

class Animation
{
private:
    const std::string id_;
    double period_;
    std::pair< double, double >* scales_;
    size_t scales_size_;
    std::pair< double, Vector3 >* angles_;
    size_t angles_size_;
    std::pair< double, Vector3 >* positions_;
    size_t positions_size_;
    TheCompletion::Method completion_method_;

public:
    Animation(const std::string& id);
    ~Animation();
    void period(double new_value);
    void scales(const std::vector< double >& scales);
    void angles(char axis, const std::vector< double >& angles);
    void positions(char axis, const std::vector< double >& positions);
    void completion_method(const std::string& method_id);

    double scale_at(double time) const;
    Vector3 angle_at(double time) const;
    Vector3 position_at(double time) const;
    bool has_scale_completion() const;
    bool has_angle_completion() const;
    bool has_position_completion() const;
};

} // namespace GraphicsDatabase

#endif
