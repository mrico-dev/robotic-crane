#pragma once

#include <cmath>

namespace simulation
{

static bool equal(float a, float b, float epsilon=1e-5f) {
    return std::abs(a - b) <= epsilon;
}

struct Crane {
    Crane() = default;
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

    void normalize_angle() {
        swing_rotation_ = std::fmod(swing_rotation_, 360);
        elbow_rotation_ = std::fmod(elbow_rotation_, 360);
        wrist_rotation_ = std::fmod(wrist_rotation_, 360);
    }

    float lift_elevation_;  // millimeters
    float swing_rotation_;  // degrees
    float elbow_rotation_;  // degrees
    float wrist_rotation_;  // degrees
    float grip_extension_;  // milimeters

    static const auto default_crane() { return Crane{5000, 0, 0, 0, 500}; }
};

struct CranePosition {
    float x_;  // millimeters
    float y_;  // millimeters
    float z_;  // millimeters
    float rotation_; // degrees
};

struct CranePhysics {
    float lift_elevation_speed_;  // millimeters / s
    float swing_rotation_speed_;  // degrees / s
    float elbow_rotation_speed_;  // degrees / s
    float wrist_rotation_speed_;  // degrees / s
    float grip_extension_speed_;  // milimeters / s
};

struct CraneMovementConfig {
    float lift_elevation_max_speed_;  // millimeters / s
    float lift_elevation_acceleration_;  // millimeters / s^2

    float swing_rotation_max_speed_;  // degrees / s
    float swing_rotation_acceleration_;  // degrees / s^2

    float elbow_rotation_max_speed_;  // degrees / s
    float elbow_rotation_acceleration_;  // degrees / s^2

    float wrist_rotation_max_speed_;  // degrees / s
    float wrist_rotation_acceleration_;  // degrees / s^2

    float grip_extension_max_speed_;  // millimeters / s
    float grip_extension_acceleration_;  // millimeters / s^2
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

struct Position {
    float x_;
    float y_;
    float z_;
};

struct PolarPosition {
    float radius_;
    float angle_;
    float y_;
};

struct CraneSpeed {
    float speed_x_;  // millimiters / s
    float speed_y_;  // millimiters / s
    float speed_z_;  // millimiters / s
    float speed_y_rotation_;  // degree / s
};

static constexpr simulation::CraneMovementConfig default_crane_movement_config = simulation::CraneMovementConfig{
                                                                                500, 100, 
                                                                                20, 8,
                                                                                10, 2,
                                                                                8, 1,
                                                                                10, 2};

static constexpr simulation::CraneShapeConfig default_crane_shape_config = simulation::CraneShapeConfig{
                                                                                1000,  // base height
                                                                                10000, // lift_height
                                                                                6000,  // arm_length
                                                                                1000,  // elbow_y
                                                                                4000,  // foream_length
                                                                                300,   // wrist_y 
                                                                                700    // wrist_length
                                                                                };

} // namespace robotics