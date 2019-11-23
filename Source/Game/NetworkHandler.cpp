#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

namespace Handler
{
Network::Network( Context* context ) : 
    Impl( context ),
    networks{},
    serverConnections{},
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
    //Create Network Instances
    for( int i = 0; i < 2; i++ )
    {
        SharedPtr<Urho3D::Network> newNetwork( new Urho3D::Network( context_ ) );
        networks.Push( newNetwork );
    }

    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( Network, HandleClientIdentity ) );
    SubscribeToEvent( E_CLIENTCONNECTED, URHO3D_HANDLER( Network, HandleClientConnected ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( Network, HandleClientDisconnected ) );
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( Network, HandleMessage ) );

    return true;
}

void Network::UnInit()
{
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
    Close( loginServerConnection );
    loginServerConnection = nullptr;
}

void Network::CloseMasterServer()
{
    Close( masterServerConnection );
    masterServerConnection = nullptr;
}

void Network::CloseGameServer()
{
    Close( gameServerConnection );
    gameServerConnection = nullptr;
}

Connection* Network::Connect( const String& ip, unsigned int port, VariantMap& identity )
{
    auto network = GetSubsystem<Urho3D::Network>();

    //Find a empty network instance
    for( auto p : networks )
    {
        if( p->GetServerConnection() == nullptr )
        {
            network = p;
            break;
        }
    }

    //Make server connection
    if( auto serverConnection = network->Connect( ip, port, nullptr, identity ); serverConnection )
    {
        serverConnections[serverConnection] = network;
        return serverConnection;
    }

    return nullptr;
}

void Network::Close( Connection* connection )
{
    if( connection )
    {
        auto it = serverConnections.Find( connection );

        if( it != serverConnections.End() )
            it->second_->Disconnect();
    }
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