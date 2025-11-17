#include "ParkingSpots.h"

#include "CleoFunctions.h"

std::vector<ParkingSpot*> g_spots;

int g_checkTimer = 0;

void ParkingSpots::Initialize()
{
    LoadSpots();

    CheckSpots();
}

void ParkingSpots::LoadSpots()
{
    std::string folder = modData->GetFile("/vehicles/"); 

    if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder))
        return;

    // percorre todos os arquivos da pasta
    for (auto& entry : std::filesystem::directory_iterator(folder))
    {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() != ".txt")
            continue;

        std::ifstream in(entry.path());
        if (!in.is_open())
            continue;

        std::string line;
        
        while (std::getline(in, line))
        {
            if (line.empty()) continue;
            if (line[0] == '#') continue;

            ParkingSpot* spot = new ParkingSpot();

            if (sscanf(line.c_str(), "%d, %f, %f, %f, %f",
                &spot->modelId, &spot->x, &spot->y, &spot->z, &spot->angle) == 5)
            {
                g_spots.push_back(spot);
            }
        }
    }
}

void ParkingSpots::Update()
{
    g_checkTimer += menuSZK->deltaTime;

    if (g_checkTimer < 5000)
        return;

    // Resetar timer para próxima rodada
    g_checkTimer = 0;

    CheckSpots();
}

void ParkingSpots::CheckSpots()
{
    // Resetar timer para próxima rodada
    g_checkTimer = 0;

    auto playerPosition = GetPlayerPosition();

    // Agora sim, executar verificação dos spots
    for (auto spot : g_spots)
    {
        CVector spotPosition = CVector(spot->x, spot->y, spot->z);

        if(distanceBetweenPoints(playerPosition, spotPosition) > 100)
            continue;

        if (spot->lastVehicleRef != -1)
        {
            if (!CAR_DEFINED(spot->lastVehicleRef))
                spot->lastVehicleRef = -1;
            // else
            //     continue;
        }

        if (VehicleExistsNearby(spot->modelId, spot->x, spot->y, spot->z, 2.0f))
            continue;

        int car = CREATE_CAR_AT(spot->modelId, spot->x, spot->y, spot->z);

        SET_CAR_Z_ANGLE(car, spot->angle);
        SET_CAR_DOOR_STATUS(car, 1);
        REMOVE_REFERENCES_TO_CAR(car);

        spot->lastVehicleRef = car;
    }
}

bool ParkingSpots::VehicleExistsNearby(int modelId, float x, float y, float z, float maxDist)
{
    for (int ref : g_vehicleRefs)
    {
        if (!CAR_DEFINED(ref))
            continue;

        if (GET_CAR_MODEL(ref) != modelId)
            continue;

        auto carPos = GetCarPosition(ref);

        float dx = carPos.x - x;
        float dy = carPos.y - y;
        float dz = carPos.z - z;

        float dist = sqrtf(dx*dx + dy*dy + dz*dz);

        if (dist <= maxDist)
            return true;
    }

    return false;
}