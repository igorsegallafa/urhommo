#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

NetworkHandler::NetworkHandler( Context* context ) :
    HandlerImpl( context )
{
    netServer = new Net::Server( context );
    messageHandler = new Handler::Message( context );
}

NetworkHandler::~NetworkHandler()
{
    netServer = nullptr;
    messageHandler = nullptr;
}

bool NetworkHandler::Init()
{
    //Initialize Net Server
    netServer->Init();
    netServer->Start( CONFIGMANAGER->GetNetConnection() );
    netServer->Load( CONFIGMANAGER->GetNetConfig( Net::ServerType::Master ) );
    netServer->ConnectAll();

    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( NetworkHandler, HandleClientIdentity ) );
    SubscribeToEvent( E_CLIENTCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleClientConnected ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleClientDisconnected ) );
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( NetworkHandler, HandleMessage ) );

    return true;
}

void NetworkHandler::UnInit()
{
    netServer->UnInit();
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