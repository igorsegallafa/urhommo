#include "PrecompiledHeader.h"
#include "ServerManager.h"

namespace Manager
{
void Server::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new Server( context ) );
}

Server::Server( Context* context ) :
    Impl( context )
{
    IMPL_MANAGER( User );
}

Server::~Server()
{
    managers.Clear();
}

bool Server::Init()
{
    for( const auto& m : managers )
        m.second_->Init();

    return true;
}

void Server::UnInit()
{
    for( const auto& m : managers )
        m.second_->UnInit();
}
}