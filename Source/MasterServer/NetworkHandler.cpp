#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

namespace Handler
{
Network::Network( Context* context ) :
    Impl( context )
{
    netServer = new Net::Server( context );
    messageHandler = new Handler::Message( context );
}

Network::~Network()
{
    netServer = nullptr;
    messageHandler = nullptr;
}

bool Network::Init()
{
    //Initialize Net Server
    netServer->Init();
    netServer->Start( Net::ServerType::Master );

    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( Network, HandleClientIdentity ) );
    SubscribeToEvent( E_CLIENTCONNECTED, URHO3D_HANDLER( Network, HandleClientConnected ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( Network, HandleClientDisconnected ) );
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( Network, HandleMessage ) );

    return true;
}

void Network::UnInit()
{
    netServer->UnInit();
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