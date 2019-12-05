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
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( NetworkHandler, HandleClientIdentity ) );
    SubscribeToEvent( E_CLIENTCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleClientConnected ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleClientDisconnected ) );
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( NetworkHandler, HandleMessage ) );

    //Handlers
    messageHandler->Handle( MSGID_LoginData ).Process( HANDLE_MESSAGE( &LoginHandler::HandleLoginData, LOGINHANDLER ) );
    messageHandler->Handle( MSGID_GameServerConnected ).Process( HANDLE_MESSAGE( &LoginHandler::HandleGameServerConnected, LOGINHANDLER ) );

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
    CloseLoginServer();
    CloseMasterServer();
    CloseGameServer();
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

void NetworkHandler::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
}

void NetworkHandler::HandleClientConnected( StringHash eventType, VariantMap& eventData )
{
}

void NetworkHandler::HandleClientDisconnected( StringHash eventType, VariantMap& eventData )
{
}

void NetworkHandler::HandleMessage( StringHash eventType, VariantMap& eventData )
{
}