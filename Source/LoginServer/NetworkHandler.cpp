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

    //Global Validation
    messageHandler->AddValidation( std::bind( &NetworkHandler::CanProcessMessage, this, std::placeholders::_1, std::placeholders::_2 ) );

    //Handlers
    messageHandler->Handle( MSGID_CreateCharacter ).Process( HANDLE_MESSAGE( &AccountHandler::HandleCreateCharacter, ACCOUNTHANDLER ) );

    return true;
}

void NetworkHandler::UnInit()
{
    netServer->UnInit();
}

bool NetworkHandler::CanProcessMessage( int messageID, Connection* connection )
{
    //Receiving Net Message from non net connection? Doesn't accept it!
    if( messageID > Net::MessageID::MSGID_None && !connection->IsNetConnection() )
    {
        URHO3D_LOGERROR( "Connection " + connection->ToString() + " tried to send a packet as Net Server!" );
        return false;
    }

    return true;
}

void NetworkHandler::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
}

void NetworkHandler::HandleClientConnected( StringHash eventType, VariantMap& eventData )
{
}

void NetworkHandler::HandleClientDisconnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());

    if( connection )
    {
        //Remove User from Server Memory
        USERMANAGER->DelUser( connection );
    }
}

void NetworkHandler::HandleMessage( StringHash eventType, VariantMap& eventData )
{
}