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
    IMPL_MANAGER( Screen );
    IMPL_MANAGER( Camera );
}

Game::~Game()
{
    managers.Clear();
}

bool Game::Init()
{
    for( const auto& m : managers )
        m.second_->Init();

    return true;
}

void Game::UnInit()
{
    for( const auto& m : managers )
        m.second_->UnInit();
}
}