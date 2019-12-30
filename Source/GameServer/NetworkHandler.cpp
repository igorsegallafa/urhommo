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
    messageHandler->Handle( MSGID_WorldData ).Process( HANDLE_MESSAGE( &UserHandler::HandleWorldData, USERHANDLER ) );
    messageHandler->Handle( MSGID_ChatGame ).Process( HANDLE_MESSAGE( &ChatHandler::HandleChatGame, CHATHANDLER ) );

    //Net Messages
    messageHandler->Handle( Net::MSGID_LoadUser ).Process( HANDLE_MESSAGE( &UserHandler::HandleLoadUser, USERHANDLER ) );

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
        return false;

    return true;
}

void NetworkHandler::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
}

void NetworkHandler::HandleClientConnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientConnected::P_CONNECTION].GetPtr());

    if( connection )
    {
        //Add User to Server Memory
        auto user = USERMANAGER->AddUser( connection );

        //Send the message to confirm the game server was connected
        connection->Send( MSGID_GameServerConnected, true, true, VectorBuffer() );
    }
}

void NetworkHandler::HandleClientDisconnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientDisconnected::P_CONNECTION].GetPtr());

    if( connection )
    {
        //Remove User from Server Memory
        USERMANAGER->DelUser( connection );
    }
}

void NetworkHandler::HandleMessage( StringHash eventType, VariantMap& eventData )
{
}