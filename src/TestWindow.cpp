#include "TestWindow.h"

#include "CleoFunctions.h"
#include "ModelLoader.h"

bool freezingCriminals = false;

void TestWindow::OpenWindow()
{
    auto window = menuSZK->CreateWindow(200, 200, 800, "Bodycam - Test");

    {
        auto button = window->AddButton("Test option");
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