#pragma once

#include "robotic_crane.hpp"

#include <cstdint>

namespace simulation {

class CraneSimulator {
    static constexpr int32_t SECOND_IN_NANOS = 1'000'000'000;
public:
    CraneSimulator(const crane& intial_state, const crane_config& crane_config);

    const crane& get_state() const;

    void simulate_next_step(int64_t elapsed_time);

    void set_goal_state(const crane& goal_crane);

private:
    static void simulate_element(float& position, float goal_position, float& speed, float max_speed, float acceleration, float seconds_elapsed);

    crane crane_;
    crane goal_crane_;
    crane_physics crane_physics_;
    crane_config crane_config_;
};

} // namespace robotics
