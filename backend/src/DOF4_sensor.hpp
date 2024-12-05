#pragma once

#include "datamodels.hpp"

namespace simulation {

class DOF4Sensor {
public:
    DOF4Sensor();

    CranePosition get_crane_position() const;

private:
    CranePosition current_position_;
};

}  // namespace simulation