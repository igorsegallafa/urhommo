#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

namespace Handler
{
Network::Network( Context* context ) : 
    Impl( context ),
    loginServerConnection( nullptr ),
    masterServerConnection( nullptr ),
    gameServerConnection( nullptr )
{
    messageHandler = new Handler::Message( context );
}

Network::~Network()
{
    messageHandler = nullptr;
}

bool Network::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( Network, HandleClientIdentity ) );
    SubscribeToEvent( E_CLIENTCONNECTED, URHO3D_HANDLER( Network, HandleClientConnected ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( Network, HandleClientDisconnected ) );
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( Network, HandleMessage ) );

    //Handlers
    messageHandler->Handle( Shared::Network::MSGID_LoginData ).Process( HANDLE_MESSAGE( &Handler::Login::HandleLoginData, LOGINHANDLER ) );

    return true;
}

void Network::UnInit()
{
    CloseConnections();
}

void Network::ConnectLoginServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        loginServerConnection = connection;
}

void Network::ConnectMasterServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        masterServerConnection = connection;
}

void Network::ConnectGameServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        gameServerConnection = connection;
}

void Network::CloseConnections()
{
    CloseLoginServer();
    CloseMasterServer();
    CloseGameServer();
}

void Network::CloseLoginServer()
{
    if( loginServerConnection )
    {
        loginServerConnection->Disconnect();
        loginServerConnection = nullptr;
    }
}

void Network::CloseMasterServer()
{
    if( masterServerConnection )
    {
        masterServerConnection->Disconnect();
        masterServerConnection = nullptr;
    }
}

void Network::CloseGameServer()
{
    if( gameServerConnection )
    {
        gameServerConnection->Disconnect();
        gameServerConnection = nullptr;
    }
}

Connection* Network::Connect( const String& ip, unsigned int port, VariantMap& identity )
{
    //Make server connection
    if( auto serverConnection = NETWORK->Connect( ip, port, nullptr, identity ); serverConnection )
        return serverConnection;

    return nullptr;
}

void Network::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
}

void Network::HandleClientConnected( StringHash eventType, VariantMap& eventData )
{
}

void Network::HandleClientDisconnected( StringHash eventType, VariantMap& eventData )
{
}

void Network::HandleMessage( StringHash eventType, VariantMap& eventData )
{
}
}