#include "PrecompiledHeader.h"
#include "LoginHandler.h"

LoginHandler::LoginHandler( Context* context ) :
    HandlerImpl( context ),
    worldServerList_{},
    worldServerIndex_(0)
{
}

LoginHandler::~LoginHandler()
{
    worldServerIndex_ = 0;
    characterList_.Clear();
    worldServerList_.Clear();
}

void LoginHandler::ProcessLogin( const String& account, const String& password )
{
    using namespace Login;

    if( account.Length() && password.Length() )
    {
        VariantMap identity;
        identity[P_ACCOUNTNAME] = account;
        identity[P_PASSWORD] = password;
        NETWORKHANDLER->ConnectLoginServer( "127.0.0.1", 10006, identity );
    }
}

void LoginHandler::ProcessWorldServer( unsigned int serverIndex )
{
    worldServerIndex_ = serverIndex;

    if( worldServerIndex_ < worldServerList_.Size() )
    {
        auto worldServer = worldServerList_[worldServerIndex_];

        VariantMap identity;
        NETWORKHANDLER->ConnectWorldServer( worldServer.ip, worldServer.port, identity );
    }
}

void LoginHandler::ProcessGameServer()
{
    if( worldServerIndex_ < worldServerList_.Size() )
    {
        auto worldServer = worldServerList_[worldServerIndex_];

        VariantMap identity;
        NETWORKHANDLER->ConnectGameServer( worldServer.gameServerIp, worldServer.gameServerPort, identity );
    }
}

void LoginHandler::ProcessMasterServer()
{
    VariantMap identity;
    NETWORKHANDLER->ConnectMasterServer( masterServerInfo_.ip, masterServerInfo_.port, identity );
}

bool LoginHandler::HandleLoginData( Connection* connection, MemoryBuffer& message ) //@MSGID_LoginData
{
    if( SCREEN_TYPE == ScreenType::Login )
    {
        StringVector worldServerNameList;
        worldServerList_.Clear();
        characterList_.Clear();

        //Get Login Status
        LoginStatus loginStatus = static_cast<LoginStatus>(message.ReadInt());

        //Successful Login
        if( loginStatus & LoginStatus::Successful )
        {
            //Read World Servers
            int totalServers = message.ReadInt();
            for( int i = 0; i < totalServers; i++ )
            {
                ServerInfo worldServer;
                worldServer.name = message.ReadString();
                worldServer.ip = message.ReadString();
                worldServer.port = message.ReadInt();
                worldServer.isOnline = message.ReadBool();
                worldServer.gameServerIp = message.ReadString();
                worldServer.gameServerPort = message.ReadInt();
                worldServerList_.Push( worldServer );

                worldServerNameList.Push( worldServer.name );
            }

            //Read Character List
            unsigned charactersFound = message.ReadUInt();
            for( unsigned i = 0; i < charactersFound; i++ )
            {
                CharacterInfo characterInfo;
                characterInfo.name = message.ReadString();
                characterInfo.level = message.ReadUInt();
                characterInfo.characterClass = static_cast<CharacterClass>(message.ReadInt());
                characterInfo.armorId = message.ReadInt();
                characterInfo.headId = message.ReadInt();
                characterList_.Push( characterInfo );
            }

            //Set World Server List
            LOGINSCREEN->SetWorldServerList( worldServerNameList );

            //Read Master Server Info
            masterServerInfo_.name = "MasterServer";
            masterServerInfo_.ip = message.ReadString();
            masterServerInfo_.port = message.ReadInt();

            NOTIFICATIONTEXT->Push( "Connected!" );
        }
        else
        {
            if( loginStatus & LoginStatus::WrongAccount )
            {
                NOTIFICATIONTEXT->Push( "Wrong account. Try again!" );
            }
            else if( loginStatus & LoginStatus::WrongPassword )
            {
                NOTIFICATIONTEXT->Push( "Wrong password. Try again!" );
            }
            else if( loginStatus & LoginStatus::AlreadyIngame )
            {
                NOTIFICATIONTEXT->Push( "Account it is already online" );
            }
        }
    }

    return true;
}

bool LoginHandler::HandleGameServerConnected( Connection* connection, MemoryBuffer& message ) //@MSGID_GameServerConnected
{
    NOTIFICATIONTEXT->Reset();

    //Change Screen for Character Select
    SCREENMANAGER->ChangeScreen( ScreenType::Character );

    return true;
}
