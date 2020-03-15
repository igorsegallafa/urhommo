#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

NetworkHandler::NetworkHandler( Context* context ) :
    HandlerImpl( context ),
    loginServerConnection( nullptr ),
    masterServerConnection( nullptr ),
    gameServerConnection( nullptr )
{
    messageHandler = new Handler::Message( context );
}

NetworkHandler::~NetworkHandler()
{
    messageHandler = nullptr;
}

bool NetworkHandler::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_SERVERCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleServerConnected ) );
    SubscribeToEvent( E_SERVERDISCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleServerDisconnected ) );

    //Handlers
    messageHandler->Handle( MSGID_LoginData ).Process( HANDLE_MESSAGE( &LoginHandler::HandleLoginData, LOGINHANDLER ) );
    messageHandler->Handle( MSGID_GameServerConnected ).Process( HANDLE_MESSAGE( &LoginHandler::HandleGameServerConnected, LOGINHANDLER ) );
    messageHandler->Handle( MSGID_WorldData ).Process( HANDLE_MESSAGE( &CharacterHandler::HandleWorldData, CHARACTERHANDLER ) );
    messageHandler->Handle( MSGID_ChatGame ).Process( HANDLE_MESSAGE( &ChatHandler::HandleChatGame, CHATHANDLER ) );

    return true;
}

void NetworkHandler::UnInit()
{
    CloseConnections();
}

void NetworkHandler::ConnectLoginServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        loginServerConnection = connection;
}

void NetworkHandler::ConnectMasterServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        masterServerConnection = connection;
}

void NetworkHandler::ConnectGameServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        gameServerConnection = connection;
}

void NetworkHandler::CloseConnections()
{
    NETWORK->Disconnect();
}

void NetworkHandler::CloseLoginServer()
{
    if( loginServerConnection )
    {
        loginServerConnection->Disconnect();
        loginServerConnection = nullptr;
    }
}

void NetworkHandler::CloseMasterServer()
{
    if( masterServerConnection )
    {
        masterServerConnection->Disconnect();
        masterServerConnection = nullptr;
    }
}

void NetworkHandler::CloseGameServer()
{
    if( gameServerConnection )
    {
        gameServerConnection->Disconnect();
        gameServerConnection = nullptr;
    }
}

Connection* NetworkHandler::Connect( const String& ip, unsigned int port, VariantMap& identity )
{
    //Make server connection
    if( auto serverConnection = NETWORK->Connect( ip, port, nullptr, identity ); serverConnection )
        return serverConnection;

    return nullptr;
}

void NetworkHandler::HandleServerConnected( StringHash eventType, VariantMap& eventData )
{
    //Game Server Connected? Connect to Master Server
    if( (gameServerConnection) && gameServerConnection->IsConnected() && !gameServerConnection->IsConnectPending() )
        LOGINHANDLER->ProcessMasterServer();
}

void NetworkHandler::HandleServerDisconnected( StringHash eventType, VariantMap& eventData )
{
    using namespace ServerDisconnected;

    if( (loginServerConnection) && loginServerConnection->GetAddressOrGUIDHash() == eventData[P_ADDRESS].GetInt() )
        loginServerConnection = nullptr;
    else if( (gameServerConnection) && gameServerConnection->GetAddressOrGUIDHash() == eventData[P_ADDRESS].GetInt() )
        gameServerConnection = nullptr;
    else if( (masterServerConnection) && masterServerConnection->GetAddressOrGUIDHash() == eventData[P_ADDRESS].GetInt() )
        masterServerConnection = nullptr;
    
    if( SCREENMANAGER->GetActiveScreenType() == ScreenType::World )
    {
        //Disconnect all servers and go back to Login Screen
        CloseConnections();
        SCREENMANAGER->ChangeScreen( ScreenType::Login );
    }
}
