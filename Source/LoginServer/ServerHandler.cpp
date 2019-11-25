#include "PrecompiledHeader.h"
#include "ServerHandler.h"

namespace Handler
{
void Server::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new Server( context ) );
}

Server::Server( Context* context ) :
    Impl( context )
{
    IMPL_HANDLER( Network );
    IMPL_HANDLER( Login );
}

Server::~Server()
{
    handlers.Clear();
}

bool Server::Init()
{
    for( const auto& m : handlers )
        m.second_->Init();

    return true;
}

void Server::UnInit()
{
    for( const auto& m : handlers )
        m.second_->UnInit();
}
}