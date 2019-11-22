#include "PrecompiledHeader.h"
#include "CharacterScreen.h"

namespace Core
{
CharacterScreen::CharacterScreen( Context* context ) : Screen( context )
{
    Init();
}

CharacterScreen::~CharacterScreen()
{
}

void CharacterScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();
}

void CharacterScreen::CreateScene()
{
}

void CharacterScreen::SetupViewport()
{
}

void CharacterScreen::BuildWindow()
{
}
}