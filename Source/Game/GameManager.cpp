#include "PrecompiledHeader.h"
#include "GameManager.h"

namespace Manager
{
void Game::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new Game( context ) );
}

Game::Game( Context* context ) :
    Impl( context )
{
    screenManager = new Screen( context );
}

Game::~Game()
{
    screenManager = nullptr;
}

bool Game::Init()
{
    screenManager->Init();

    return true;
}

void Game::UnInit()
{
    screenManager->UnInit();
}
}