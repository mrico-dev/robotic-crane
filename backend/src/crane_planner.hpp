#pragma once

#include "datamodels.hpp"

namespace simulation {

class CranePlanner {
public:
    CranePlanner(const CraneShapeConfig& config);

    Crane get_target_crane(const Position& pos);

private:
    PolarPosition get_polar_coords(const Position& pos);

    float rad_to_degrees(float rad);

    CraneShapeConfig config_;
};
    
} // namespace simulation
