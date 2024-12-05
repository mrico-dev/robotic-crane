#include "DOF4_sensor.hpp"

namespace simulation
{

DOF4Sensor::DOF4Sensor()
{
    current_position_ = {1000, 6000, 1000, 45};
}

CranePosition DOF4Sensor::get_crane_position() const
{
    // We could imagine this class does some async computation, and refresh "current position"
    // with its latest measurements
    return current_position_;
}

} // namespace simulation
