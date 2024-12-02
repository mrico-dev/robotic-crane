#pragma once

#include "datamodels.hpp"

#include <cstdint>

namespace simulation {

class CraneSimulator {
    static constexpr int32_t SECOND_IN_NANOS = 1'000'000'000;
public:
    CraneSimulator(const Crane& intial_state, const CraneMovementConfig& crane_config);

    const Crane& get_state() const;

    void simulate_next_step(int64_t elapsed_time);

    void set_goal_state(const Crane& goal_crane);

private:
    static void simulate_element(float& position, float goal_position, float& speed, float max_speed, float acceleration, float seconds_elapsed);

    Crane crane_;
    Crane goal_crane_;
    CranePhysics crane_physics_;
    CraneMovementConfig crane_config_;
};

} // namespace robotics
