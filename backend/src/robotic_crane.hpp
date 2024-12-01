#pragma once

namespace robotics
{

class crane {
public:
    crane(float lift_elevation, float swing_rotation, float elbow_rotation, float wrist_rotation, float grip_extension);

    float lift_elevation_;  // milimeters
    float swing_rotation_;  // degrees
    float elbow_rotation_;  // degrees
    float wrist_rotation_;  // degrees
    float grip_extenstion_;  // milimeters
};

} // namespace robotics