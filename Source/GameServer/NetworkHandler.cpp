#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

namespace Handler
{
Network::Network( Context* context_ ) : 
    Object( context_ )
{
}

Network::~Network()
{
}

bool Network::Init()
{
    //Initialize Net Server
    NETSERVER->Init();
    NETSERVER->Start( Net::ServerType::Game );
    NETSERVER->Load( Net::ServerType::Master );
    NETSERVER->ConnectAll();

    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( Network, HandleClientIdentity ) );
    SubscribeToEvent( E_CLIENTCONNECTED, URHO3D_HANDLER( Network, HandleClientConnected ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( Network, HandleClientDisconnected ) );

    return true;
}

void Network::UnInit()
{
    NETSERVER->UnInit();
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
}