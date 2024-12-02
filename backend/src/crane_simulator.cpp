#include "crane_simulator.hpp"

namespace simulation {
    
CraneSimulator::CraneSimulator(const crane& intial_state, const crane_config& crane_config):
    crane_(intial_state),
    goal_crane_(intial_state),
    crane_config_(crane_config) {}

const crane &CraneSimulator::get_state() const {
    return crane_;
}

void CraneSimulator::simulate_element(float& position, float goal_position, float& speed, float max_speed, float acceleration, float seconds_elapsed) {
    if (!equal(position, goal_position)) {
        if (position < goal_position) {
            if (speed < max_speed) {
                speed = std::min(speed + acceleration * seconds_elapsed, max_speed);
            }
            position = std::min(position + speed * seconds_elapsed, goal_position);
        } else {
            if (speed > -max_speed) {
                speed = std::max(speed - acceleration * seconds_elapsed, -max_speed);
            }
            position = std::max(position + speed * seconds_elapsed, goal_position);
        }
    }
}

void CraneSimulator::simulate_next_step(int64_t elapsed_time) {
    float seconds_elapsed = static_cast<double>(elapsed_time) / SECOND_IN_NANOS;

    simulate_element(crane_.lift_elevation_,
                     goal_crane_.lift_elevation_,
                     crane_physics_.lift_elevation_speed_,
                     crane_config_.lift_elevation_max_speed_,
                     crane_config_.lift_elevation_acceleration_,
                     seconds_elapsed);
    simulate_element(crane_.swing_rotation_,
                     goal_crane_.swing_rotation_,
                     crane_physics_.swing_rotation_speed_,
                     crane_config_.swing_rotation_max_speed_,
                     crane_config_.swing_rotation_acceleration_,
                     seconds_elapsed);
    simulate_element(crane_.elbow_rotation_,
                     goal_crane_.elbow_rotation_,
                     crane_physics_.elbow_rotation_speed_,
                     crane_config_.elbow_rotation_max_speed_,
                     crane_config_.elbow_rotation_acceleration_,
                     seconds_elapsed);
    simulate_element(crane_.wrist_rotation_,
                     goal_crane_.wrist_rotation_,
                     crane_physics_.wrist_rotation_speed_,
                     crane_config_.wrist_rotation_max_speed_,
                     crane_config_.wrist_rotation_acceleration_,
                     seconds_elapsed);
    simulate_element(crane_.grip_extension_,
                     goal_crane_.grip_extension_,
                     crane_physics_.grip_extension_speed_,
                     crane_config_.grip_extension_max_speed_,
                     crane_config_.grip_extension_acceleration_,
                     seconds_elapsed);
}

void CraneSimulator::set_goal_state(const crane &goal_crane) {
    // TODO Normalize angles so that we don't rotate 659 degrees to from 659 to 0
    goal_crane_ = goal_crane;
}

} // namespace simulation