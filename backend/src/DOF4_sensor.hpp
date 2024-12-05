#pragma once

#include "datamodels.hpp"

namespace simulation {

class DOF4Sensor {
public:
    DOF4Sensor();

    void set_crane_speed(const CraneSpeed& crane_speed);

    CranePosition get_next_crane_position(float time_elapsed_second);

private:
    CraneSpeed crane_speed_;
    CranePosition current_position_;
};

}  // namespace simulation