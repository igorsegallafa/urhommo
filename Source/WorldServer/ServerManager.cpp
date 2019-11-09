#include "PrecompiledHeader.h"
#include "ServerManager.h"

namespace Manager
{
void Server::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new Server( context ) );
}

Server::Server( Context* context_ ) : 
    Object( context_ )
{
}

Server::~Server()
{
}

bool Server::Init()
{
    return true;
}

void Server::UnInit()
{
}
}