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
    IMPL_HANDLER( LoginHandler );
    IMPL_HANDLER( AccountHandler );
}

ServerHandler::~ServerHandler()
{
    handlers.Clear();
}

bool ServerHandler::Init()
{
    for( const auto& m : handlers )
        m.second_->Init();

    return true;
}

void ServerHandler::UnInit()
{
    for( const auto& m : handlers )
        m.second_->UnInit();
}