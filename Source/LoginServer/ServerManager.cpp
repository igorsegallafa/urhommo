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
    userManager = new User( context );
}

Server::~Server()
{
    userManager = nullptr;
}

bool Server::Init()
{
    return true;
}

void Server::UnInit()
{
}
}