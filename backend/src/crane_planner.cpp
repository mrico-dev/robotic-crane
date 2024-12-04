#include "crane_planner.hpp"

#include <cmath>
#include <iostream>

namespace simulation
{
    constexpr float PI = 3.141592653589793238462643383279502884;

    CranePlanner::CranePlanner(const CraneShapeConfig &config): config_(config) {}

    Crane simulation::CranePlanner::get_target_crane(const Position& pos) {
        const auto polar_pos = get_polar_coords(pos);
        const auto triangle_length = polar_pos.radius_ - config_.wrist_length_;

        if (config_.arm_length_ + config_.forearm_length_ < triangle_length) {
            std::cerr << "Requested position is too far away from the crane. Going back to default position instead." << std::endl;
            return Crane::default_crane();
        }
        if (triangle_length < std::abs(config_.arm_length_ - config_.forearm_length_)) {
            std::cerr << "Requested position is too close to the crane. Going back to default position instead." << std::endl;
            return Crane::default_crane();
        }
        auto lift = pos.y_ + config_.elbow_spacing_y_ + config_.wrist_spacing_y_;
        if (lift > config_.lift_height_) {
            std::cerr << "Requested position is too high. Going back to default position instead." << std::endl;
            return Crane::default_crane();
        }

        const auto a = config_.arm_length_;
        const auto b = config_.forearm_length_;
        const auto c = triangle_length;

        // Using c^2 = a^2 + b^2 - 2ab cos(^ba)  =>  cos(^ba) = (a^2 + b^2 - c^2) / 2ab (Law of cosines)
        const auto swing_angle = std::acos((a * a + c * c - b * b) / (2 * a * c));
        const auto elbow_rotation = -PI + std::acos((a * a + b * b - c * c) / (2 * a * b));
        const auto wrist_rotation = -swing_angle - elbow_rotation;
        const auto swing_rotation = swing_angle + polar_pos.angle_;

        auto grip = 150.f;

        return Crane{lift, rad_to_degrees(swing_rotation), rad_to_degrees(elbow_rotation), rad_to_degrees(wrist_rotation), grip};
    }

    PolarPosition CranePlanner::get_polar_coords(const Position &pos) {
        auto radius = std::sqrt(pos.x_ * pos.x_ + pos.z_ * pos.z_);
        auto angle = std::atan2(pos.z_, pos.x_);

        return {radius, angle, pos.y_};
    }

    float CranePlanner::rad_to_degrees(float rad) {
        return rad * 180 / PI;
    }

} // namespace simulation

