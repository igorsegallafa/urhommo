#include "PrecompiledHeader.h"
#include "Server.h"

namespace Net::Manager
{
void Server::RegisterServerLibrary( Context* context_ )
{
    context_->RegisterSubsystem( new Server( context_ ) );
}

Server::Server( Context* context_ ) : Object( context_ )
{
}

Server::~Server()
{
}
}