#include "PrecompiledHeader.h"
#include "Main.h"

Server::Server( Context* context ) : Application( context )
{
}

void Server::Setup()
{
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_HEIGHT] = 600;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
}

void Server::Start()
{
    GetSubsystem<Input>()->SetMouseVisible( true );
}

void Server::Stop()
{
}