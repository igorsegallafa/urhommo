#include "PrecompiledHeader.h"
#include "GameHandler.h"

void GameHandler::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new GameHandler( context ) );
}

GameHandler::GameHandler( Context* context ) :
    HandlerImpl( context )
{
    IMPL_HANDLER( LoginHandler );
    IMPL_HANDLER( NetworkHandler );
}

GameHandler::~GameHandler()
{
    handlers.Clear();
}

bool GameHandler::Init()
{
    for( const auto& m : handlers )
        m.second_->Init();

    return true;
}

void GameHandler::UnInit()
{
    for( const auto& m : handlers )
        m.second_->UnInit();
}