#include "Mod.h"

#include "CleoFunctions.h"
#include "ParkingSpots.h"

bool hasFirstUpdated = false;

void InitMod()
{
    ParkingSpots::Initialize();

    menuSZK->onVehicleAdded->Add([](int ref) {
        g_vehicleRefs.insert(ref);
    });

    menuSZK->onVehicleRemoved->Add([](int ref) {
        g_vehicleRefs.erase(ref);
    });
}

void OnGameUpdate()
{
    if(!hasFirstUpdated)
    {
        hasFirstUpdated = true;
        InitMod();
    }

    if(menuSZK->GetGlobalIntVariable("open_parkedvehicles") == 1)
    {
        menuSZK->SetGlobalIntVariable("open_parkedvehicles", 0);
        CreateWindow();
    }

    ParkingSpots::Update();
}

void CreateWindow()
{
    auto window = menuSZK->CreateWindow(400, 200, 800, "Parked Vehicles");

    {
        auto button = window->AddButton("test");
        button->onClick->Add([window]() {
            window->Close();
        });
    }

    {
        auto button = window->AddButton("~r~" + GetTranslatedText("close"));
        button->onClick->Add([window]() {
            window->Close();
        });
    }
}