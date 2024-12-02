#pragma once

#include <cmath>

namespace simulation
{

static bool equal(float a, float b, float epsilon=1e-5f) {
    return std::abs(a - b) <= epsilon;
}

struct Crane {
    Crane(float lift_elevation, float swing_rotation, float elbow_rotation, float wrist_rotation, float grip_extension):
    lift_elevation_(lift_elevation),
    swing_rotation_(swing_rotation),
    elbow_rotation_(elbow_rotation),
    wrist_rotation_(wrist_rotation),
    grip_extension_(grip_extension) {}

    bool operator==(const Crane& other) const {
        return equal(lift_elevation_, other.lift_elevation_)
            && equal(swing_rotation_, other.swing_rotation_)
            && equal(elbow_rotation_, other.elbow_rotation_)
            && equal(wrist_rotation_, other.wrist_rotation_)
            && equal(grip_extension_, other.grip_extension_);
    }

    float lift_elevation_;  // milimeters
    float swing_rotation_;  // degrees
    float elbow_rotation_;  // degrees
    float wrist_rotation_;  // degrees
    float grip_extension_;  // milimeters

    static const auto default_crane() { return Crane{5000, 0, 0, 0, 500}; }
};

struct CranePhysics {
    float lift_elevation_speed_;  // milimeters / s
    float swing_rotation_speed_;  // degrees / s
    float elbow_rotation_speed_;  // degrees / s
    float wrist_rotation_speed_;  // degrees / s
    float grip_extension_speed_;  // milimeters / s
};

struct CraneMovementConfig {
    float lift_elevation_max_speed_;  // milimeters / s
    float lift_elevation_acceleration_;  // milimeters / s

    float swing_rotation_max_speed_;  // degrees / s
    float swing_rotation_acceleration_;  // degrees / s

    float elbow_rotation_max_speed_;  // degrees / s
    float elbow_rotation_acceleration_;  // degrees / s

    float wrist_rotation_max_speed_;  // degrees / s
    float wrist_rotation_acceleration_;  // degrees / s

    float grip_extension_max_speed_;  // milimeters / s
    float grip_extension_acceleration_;  // milimeters / s
};

struct CraneShapeConfig {
    float base_height_;
    float lift_height_;
    float arm_length_;
    float elbow_spacing_y_;
    float forearm_length_;
    float wrist_spacing_y_;
    float wrist_length_;
};

struct EulerPosition {
    float x_;
    float y_;
    float z_;
};

struct PolarPosition {
    float radius_;
    float angle_;
    float z_;
};

static constexpr simulation::CraneMovementConfig default_crane_movement_config = simulation::CraneMovementConfig{
                                                                                500, 100, 
                                                                                20, 8,
                                                                                10, 2,
                                                                                8, 1,
                                                                                10, 2};

static constexpr simulation::CraneShapeConfig default_crane_shape_config = simulation::CraneShapeConfig{
                                                                                1000,
                                                                                10000,
                                                                                8000,
                                                                                1000,
                                                                                2000,
                                                                                300,
                                                                                700
                                                                                };

} // namespace robotics