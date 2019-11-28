#include "PrecompiledHeader.h"
#include "CharacterScreen.h"

CharacterScreen::CharacterScreen( Context* context ) : Screen( context )
{
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