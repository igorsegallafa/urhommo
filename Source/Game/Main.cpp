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
    engineParameters_[EP_WINDOW_TITLE] = "Game";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_HEIGHT] = 600;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
    engineParameters_[EP_WINDOW_RESIZABLE] = true;
    engineParameters_[EP_RESOURCE_PATHS] = "Data;";
}

void Game::Start()
{
    GetSubsystem<Input>()->SetMouseVisible( true );

    GAMEMANAGER->Init();
    GAMEHANDLER->Init();
}

void Game::Stop()
{
    GAMEHANDLER->UnInit();
    GAMEMANAGER->UnInit();
}