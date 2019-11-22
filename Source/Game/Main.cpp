#include "PrecompiledHeader.h"
#include "Main.h"

Game::Game( Context* context ) : Application( context )
{
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
}

void Game::Stop()
{
}