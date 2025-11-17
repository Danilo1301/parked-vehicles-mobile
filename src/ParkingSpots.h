#pragma once

#include "pch.h"

struct ParkingSpot
{
    int modelId;
    float x, y, z;
    float angle;

    int lastVehicleRef = 0;
};

class ParkingSpots {
public:
    static void Initialize();
    static void LoadSpots();
    static void Update();
    static void CheckSpots();
    static bool VehicleExistsNearby(int modelId, float x, float y, float z, float maxDist);
};