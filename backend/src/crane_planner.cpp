#include "crane_planner.hpp"

#include <cmath>

namespace simulation
{
    constexpr float PI = 3.141592653589793238462643383279502884;

    CranePlanner::CranePlanner(const CraneShapeConfig &config): config_(config) {}

    Crane simulation::CranePlanner::get_target_crane(const EulerPosition& pos) {
        const auto polar_pos = get_polar_coords(pos);
        const auto triangle_length = polar_pos.radius_ - config_.wrist_length_;

        if (config_.arm_length_ + config_.forearm_length_ < triangle_length) {
            return Crane::default_crane();
        }
        if (polar_pos.radius_ < std::abs(config_.arm_length_ - config_.forearm_length_)) {
            // TODO edge case
            return Crane::default_crane();
        }

        const auto a = config_.arm_length_;
        const auto b = config_.forearm_length_;
        const auto c = triangle_length;
        const auto swing_angle = std::acos((b * b + c * c - a * a) / (2 * a * b));
        const auto elbow_rotation = -std::acos((a * a + b * b - c * c) / (2 * a * b));
        const auto wrist_rotation = PI - swing_angle - elbow_rotation;
        const auto swing_rotation = swing_angle + polar_pos.angle_;

        auto lift = pos.z_ - config_.base_height_ + config_.elbow_spacing_y_ + config_.wrist_spacing_y_;
        auto grip = 200.f;

        return Crane{lift, rad_to_degrees(swing_rotation), rad_to_degrees(elbow_rotation), rad_to_degrees(wrist_rotation), grip};
    }

    PolarPosition CranePlanner::get_polar_coords(const EulerPosition &pos) {
        auto radius = std::sqrt(pos.x_ * pos.x_ + pos.y_ * pos.y_);
        auto angle = std::atan2(pos.x_, pos.y_);

        return {radius, angle, pos.z_};
    }

    float CranePlanner::rad_to_degrees(float rad) {
        return rad * 180 / PI;
    }

} // namespace simulation

