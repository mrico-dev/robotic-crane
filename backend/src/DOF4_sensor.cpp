#include "DOF4_sensor.hpp"

namespace simulation
{

DOF4Sensor::DOF4Sensor(): current_position_{0, 0, 0, 0}, crane_speed_{0, 0, 0, 0} {}

void DOF4Sensor::set_crane_speed(const CraneSpeed &crane_speed) {
    crane_speed_ = crane_speed;
}

CranePosition DOF4Sensor::get_next_crane_position(float time_elapsed_second) {
    current_position_.x_ += crane_speed_.speed_x_ * time_elapsed_second;
    current_position_.y_ += crane_speed_.speed_y_ * time_elapsed_second;
    current_position_.z_ += crane_speed_.speed_z_ * time_elapsed_second;
    current_position_.rotation_ += crane_speed_.speed_y_rotation_ * time_elapsed_second;

    return current_position_;
}

} // namespace simulation
