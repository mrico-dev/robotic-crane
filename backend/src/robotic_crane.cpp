#include "robotic_crane.hpp"

namespace robotics {

crane::crane(float lift_elevation, float swing_rotation, float elbow_rotation, float wrist_rotation, float grip_extension):
    lift_elevation_(lift_elevation),
    swing_rotation_(swing_rotation),
    elbow_rotation_(elbow_rotation),
    wrist_rotation_(wrist_rotation),
    grip_extenstion_(grip_extension) {}

} // namespace robotics