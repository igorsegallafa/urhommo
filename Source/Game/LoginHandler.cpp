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

bool LoginHandler::HandleLoginData( Connection* connection, MemoryBuffer& message )
{
    if( SCREEN_TYPE == ScreenType::Login )
    {
        StringVector gameServerNameList;
        gameServerList.Clear();

        int totalServers = message.ReadInt();
        for( int i = 0; i < totalServers; i++ )
        {
            GameServerInfo gameserver;
            gameserver.name = message.ReadString();
            gameserver.ip = message.ReadString();
            gameserver.port = message.ReadInt();
            gameServerList.Push( gameserver );

            gameServerNameList.Push( gameserver.name );
        }

        LOGINSCREEN->SetGameServerList( gameServerNameList );
    }

    return true;
}

bool LoginHandler::HandleGameServerConnected( Connection* connection, MemoryBuffer& message )
{
    SCREENMANAGER->ChangeScreen( ScreenType::Character );
    return true;
}
