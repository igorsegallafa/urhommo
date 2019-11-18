#include "PrecompiledHeader.h"
#include "ServerHandler.h"

namespace Handler
{
void Server::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new Server( context ) );
}

Server::Server( Context* context_ ) : 
    Object( context_ )
{
    networkHandler = new Handler::Network( context_ );
}

Server::~Server()
{
    networkHandler = nullptr;
}

bool Server::Init()
{
    networkHandler->Init();

    return true;
}

void Server::UnInit()
{
    networkHandler->UnInit();
}
}