#include "PrecompiledHeader.h"
#include "GameHandler.h"

namespace Handler
{
void Game::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new Game( context ) );
}

Game::Game( Context* context ) :
    Impl( context )
{
}

Game::~Game()
{
}

bool Game::Init()
{
    return true;
}

void Game::UnInit()
{
}
}