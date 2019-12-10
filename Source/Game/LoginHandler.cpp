#include "PrecompiledHeader.h"
#include "LoginHandler.h"

LoginHandler::LoginHandler( Context* context ) :
    HandlerImpl( context ),
    gameServerList{}
{
}

LoginHandler::~LoginHandler()
{
}

void LoginHandler::ProcessLogin( const String& account, const String& password )
{
    if( account.Length() && password.Length() )
    {
        VariantMap identity;
        identity[Login::P_ACCOUNTNAME] = account;
        identity[Login::P_PASSWORD] = password;
        NETWORKHANDLER->ConnectLoginServer( "127.0.0.1", 52011, identity );
    }
}

void LoginHandler::ProcessGameServer( int serverIndex )
{
    if( serverIndex >= 0 && serverIndex < gameServerList.Size() )
    {
        auto gameServer = gameServerList[serverIndex];

        VariantMap identity;
        NETWORKHANDLER->ConnectGameServer( gameServer.ip, gameServer.port, identity );
    }
}

void LoginHandler::ProcessMasterServer()
{
    VariantMap identity;
    NETWORKHANDLER->ConnectMasterServer( masterServerInfo.ip, masterServerInfo.port, identity );
}

bool LoginHandler::HandleLoginData( Connection* connection, MemoryBuffer& message )
{
    if( SCREEN_TYPE == ScreenType::Login )
    {
        StringVector gameServerNameList;
        gameServerList.Clear();

        //Get Login Status
        Core::LoginStatus loginStatus = (Core::LoginStatus)message.ReadInt();

        //Successful Login
        if( loginStatus == Core::LoginStatus::Successful )
        {
            //Read Game Servers
            int totalServers = message.ReadInt();
            for( int i = 0; i < totalServers; i++ )
            {
                ServerInfo gameserver;
                gameserver.name = message.ReadString();
                gameserver.ip = message.ReadString();
                gameserver.port = message.ReadInt();
                gameServerList.Push( gameserver );

                gameServerNameList.Push( gameserver.name );
            }

            //Set Game Server List
            LOGINSCREEN->SetGameServerList( gameServerNameList );

            //Read Master Server Info
            masterServerInfo.name = "MasterServer";
            masterServerInfo.ip = message.ReadString();
            masterServerInfo.port = message.ReadInt();
        }
    }

    return true;
}

bool LoginHandler::HandleGameServerConnected( Connection* connection, MemoryBuffer& message )
{
    //TODO: Set Character List

    //Change Screen for Character Select
    SCREENMANAGER->ChangeScreen( ScreenType::Character );

    return true;
}
