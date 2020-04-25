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
    IMPL_HANDLER( UserHandler );
    IMPL_HANDLER( CharacterHandler );
    IMPL_HANDLER( ChatHandler );
}

GameHandler::~GameHandler()
{
    for( auto& handler : handlers_ )
        delete handler.second_;

    handlers_.Clear();
}

bool GameHandler::Init()
{
    for( const auto& handler : handlers_ )
        handler.second_->Init();

    return true;
}

void GameHandler::UnInit()
{
    for( const auto& handler : handlers_ )
        handler.second_->UnInit();
}