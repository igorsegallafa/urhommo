#include "PrecompiledHeader.h"
#include "WorldScreen.h"

namespace Core
{
WorldScreen::WorldScreen( Context* context ) : Screen( context )
{
    Init();
}

WorldScreen::~WorldScreen()
{
}

void WorldScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();
}

void WorldScreen::CreateScene()
{
}

void WorldScreen::SetupViewport()
{
}

void WorldScreen::BuildWindow()
{
}
}