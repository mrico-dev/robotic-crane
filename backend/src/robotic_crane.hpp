#pragma once

#include <cmath>

namespace simulation
{

static bool equal(float a, float b, float epsilon=1e-5f) {
    return std::abs(a - b) <= epsilon;
}

struct crane {
    crane(float lift_elevation, float swing_rotation, float elbow_rotation, float wrist_rotation, float grip_extension):
    lift_elevation_(lift_elevation),
    swing_rotation_(swing_rotation),
    elbow_rotation_(elbow_rotation),
    wrist_rotation_(wrist_rotation),
    grip_extension_(grip_extension) {}

    bool operator==(const crane& other) const {
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
};

struct crane_physics {
    float lift_elevation_speed_;  // milimeters / s
    float swing_rotation_speed_;  // degrees / s
    float elbow_rotation_speed_;  // degrees / s
    float wrist_rotation_speed_;  // degrees / s
    float grip_extension_speed_;  // milimeters / s
};

struct crane_config {
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

} // namespace robotics