#include "PrecompiledHeader.h"
#include "Main.h"
#include "GameHandler.h"

Game::Game( Context* context ) : Application( context )
{
    Handler::Game::RegisterLibrary( context );
    Manager::Game::RegisterLibrary( context );
}

void Game::Setup()
{
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_HEIGHT] = 600;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
}

void Game::Start()
{
    GetSubsystem<Input>()->SetMouseVisible( true );

    GAMEMANAGER->Init();
    GAMEHANDLER->Init();

    //Set Login Screen
    SCREENMANAGER->SetActiveScreen( Core::ScreenType::Login );
}

void Game::Stop()
{
    GAMEHANDLER->UnInit();
    GAMEMANAGER->UnInit();
}