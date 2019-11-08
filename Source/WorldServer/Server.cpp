#include "PrecompiledHeader.h"
#include "Server.h"

Server::Server( Context* context ) : Application( context )
{
    Net::Server::RegisterServerLibrary( context );
}

void Server::Setup()
{
    engineParameters_[EP_WINDOW_TITLE] = "Game Server";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_HEADLESS] = true;
    engineParameters_[EP_SOUND] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = false;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
    engineParameters_[EP_WINDOW_HEIGHT] = 416;
    engineParameters_[EP_RESOURCE_PATHS] = "Data;CoreData;ServerData;";
}

void Server::Start()
{
    OpenConsoleWindow();

    GetSubsystem<Net::Server>()->Init();
    GetSubsystem<Net::Server>()->Start( Net::ServerType::Game );
    GetSubsystem<Net::Server>()->Load( Net::ServerType::Master );
    GetSubsystem<Net::Server>()->ConnectAll();
}

void Server::Stop()
{
}