#include "PrecompiledHeader.h"
#include "TestScreen.h"

namespace Core
{
TestScreen::TestScreen( Context* context ) : Screen( context )
{
}

TestScreen::~TestScreen()
{
}

void TestScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();
}

void TestScreen::CreateScene()
{
}

void TestScreen::SetupViewport()
{
}

void TestScreen::BuildWindow()
{
}
}