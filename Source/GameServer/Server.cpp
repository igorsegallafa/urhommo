#include "PrecompiledHeader.h"
#include "Server.h"

#if defined(_WIN32) || defined(WIN32)
Server* serverInstance = nullptr;

BOOL ConsoleHandler( DWORD event )
{
    if( event == CTRL_CLOSE_EVENT )
    {
        serverInstance->ErrorExit();
        return TRUE;
    }
    return FALSE;
}
#endif

Server::Server( Context* context ) : Application( context )
{
    ServerManager::RegisterLibrary( context );
    ServerHandler::RegisterLibrary( context );
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
    engineParameters_[EP_RESOURCE_PATHS] = "Data;ServerData;";
    engineParameters_[EP_LOG_NAME] = "Logs/" + engineParameters_[EP_WINDOW_TITLE].GetString() + ".log";
}

void Server::Start()
{
#if defined(_WIN32) || defined(WIN32)
    serverInstance = this;
    SetConsoleCtrlHandler( (PHANDLER_ROUTINE)(ConsoleHandler), TRUE );
#endif

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
    printf( "MMMMMMM.  M M  MMMM' .M\t\tGame Server\n" );
    printf( "M. .MMM'  M M  MMP' .MM\n" );
    printf( "Mb.     .dM M     .dMMM\n" );
    printf( "MMMMMMMMMMM MMMMMMMMMMM\n\n" );
}