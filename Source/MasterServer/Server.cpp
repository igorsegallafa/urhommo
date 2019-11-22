#include "PrecompiledHeader.h"
#include "Server.h"
#include "ServerManager.h"

#include "User.h"

Server::Server( Context* context ) : Application( context )
{
    Manager::Server::RegisterLibrary( context );
    Handler::Server::RegisterLibrary( context );
}

void Server::Setup()
{
    engineParameters_[EP_WINDOW_TITLE] = "Master Server";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_HEADLESS] = true;
    engineParameters_[EP_SOUND] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = false;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
    engineParameters_[EP_WINDOW_HEIGHT] = 416;
    engineParameters_[EP_RESOURCE_PATHS] = "Data;CoreData;ServerData;";
    engineParameters_[EP_LOG_NAME] = "Logs/" + engineParameters_[EP_WINDOW_TITLE].GetString() + ".log";
}

void Server::Start()
{
    OpenConsoleWindow();

    SERVERMANAGER->Init();
    SERVERHANDLER->Init();
}

void Server::Stop()
{
    SERVERHANDLER->UnInit();
    SERVERMANAGER->UnInit();
}