#ifndef GRAPHICS_DATABASE__ANIMATION_H_
#define GRAPHICS_DATABASE__ANIMATION_H_
#include <string>
#include <vector>

namespace GraphicsDatabase
{

class Completion;
class Vector3;

class Animation
{
private:
    const std::string id_;
    Completion** scale_completions_;
    Completion** angle_completions_;
    Completion** position_completions_;

public:
    Animation(const std::string& id);
    ~Animation();
    void scale_completions( const std::string& completion_id,
                            const char axis,
                            const std::vector< double >& scales,
                            const double period);
    bool has_scale_completions() const;
    void angle_completions( const std::string& completion_id,
                            const char axis,
                            const std::vector< double >& angles,
                            const double period);
    bool has_angle_completions() const;
    void position_completions(  const std::string& completion_id,
                                const char axis,
                                const std::vector< double >& positions,
                                const double period);
    bool has_position_completions() const;

    void apply_speed_to_angle(  const char axis,
                                const double t,
                                const double speed);
    void apply_speed_to_position(   const char axis,
                                    const double t,
                                    const double speed);
    void scale_at(Vector3* scale, const double time) const;
    void angle_at(Vector3* angle, const double time) const;
    void position_at(Vector3* position, const double time) const;
    void transform_angle_polynomial3(const char axis);
    void transform_position_polynomial3(const char axis);
};

} // namespace GraphicsDatabase

#endif
