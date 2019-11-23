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
    IMPL_HANDLER( Login );
    IMPL_HANDLER( Network );
}

Game::~Game()
{
    handlers.Clear();
}

bool Game::Init()
{
    for( const auto& m : handlers )
        m.second_->Init();

    return true;
}

void Game::UnInit()
{
    for( const auto& m : handlers )
        m.second_->UnInit();
}
}