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
    Completion* scale_completion_;
    Completion** angle_completions_;
    Completion** position_completions_;

public:
    Animation(const std::string& id);
    ~Animation();
    void scale_completion(  const std::string& completion_id,
                            const std::vector< double >& scales,
                            const double period);
    bool has_scale_completion() const;
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
    void scale_at(double* scale, const double time) const;
    void angle_at(Vector3* angle, const double time) const;
    void position_at(Vector3* position, const double time) const;
};

} // namespace GraphicsDatabase

#endif
