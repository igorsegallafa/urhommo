#include "PrecompiledHeader.h"
#include "Server.h"

Server::Server( Context* context ) : Application( context )
{
    ServerManager::RegisterLibrary( context );
    ServerHandler::RegisterLibrary( context );
    Shared::SetIsGameRunning( false );
    Shared::RegisterLibrary( context );
}

void Server::Setup()
{
    engineParameters_[EP_WINDOW_TITLE] = "Master Server";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_HEADLESS] = true;
    engineParameters_[EP_SOUND] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = false;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
    engineParameters_[EP_WINDOW_HEIGHT] = 405;
    engineParameters_[EP_RESOURCE_PATHS] = "Data;ServerData;";
    engineParameters_[EP_LOG_NAME] = "Logs/" + engineParameters_[EP_WINDOW_TITLE].GetString() + ".log";
}

void Server::Start()
{
    PrintSignature();

    SERVERMANAGER->Init();
    SERVERHANDLER->Init();
}

void Server::Stop()
{
    SERVERHANDLER->UnInit();
    SERVERMANAGER->UnInit();
}

void Server::PrintSignature()
{
    printf( "\nVersion Compiled on %s %s\n", __DATE__, __TIME__ );
    printf( "(C) 2019 - 2020\n" );
    printf( "\nMP\"\"\"\"\"\"`MM M\"\"MMMMM\"\"M\n" );
    printf( "M  mmmmm..M M  MMMMM  M\n" );
    printf( "M.      `YM M  MMMMP  M\n" );
    printf( "MMMMMMM.  M M  MMMM' .M\t\tMaster Server\n" );
    printf( "M. .MMM'  M M  MMP' .MM\n" );
    printf( "Mb.     .dM M     .dMMM\n" );
    printf( "MMMMMMMMMMM MMMMMMMMMMM\n\n" );
}
