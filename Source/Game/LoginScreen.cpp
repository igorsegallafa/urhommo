#include "PrecompiledHeader.h"
#include "LoginScreen.h"

namespace Core
{
LoginScreen::LoginScreen( Context* context ) : Screen( context )
{
    Init();
}

LoginScreen::~LoginScreen()
{
}

void LoginScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();
}

void LoginScreen::CreateScene()
{
}

void LoginScreen::SetupViewport()
{
}

void LoginScreen::BuildWindow()
{
}
}