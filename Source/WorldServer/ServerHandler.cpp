#include "PrecompiledHeader.h"
#include "ServerHandler.h"

void ServerHandler::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new ServerHandler( context ) );
}

ServerHandler::ServerHandler( Context* context ) :
    HandlerImpl( context )
{
    IMPL_HANDLER( NetworkHandler );
    IMPL_HANDLER( UserHandler );
}

ServerHandler::~ServerHandler()
{
    for( auto& handler : handlers_ )
        delete handler.second_;

    handlers_.Clear();
}

bool ServerHandler::Init()
{
    for( const auto& handler : handlers_ )
        handler.second_->Init();

    return true;
}

void ServerHandler::UnInit()
{
    for( const auto& handler : handlers_ )
        handler.second_->UnInit();
}