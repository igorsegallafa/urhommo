#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

NetworkHandler::NetworkHandler( Context* context ) :
    HandlerImpl( context ),
    Handler::Message()
{
    netServer_ = new Net::Server( context );
}

NetworkHandler::~NetworkHandler()
{
    netServer_ = nullptr;
}

bool NetworkHandler::Init()
{
    //Initialize Net Server
    netServer_->Init();
    netServer_->Start( CONFIGMANAGER->GetNetConnection() );
    netServer_->Load( CONFIGMANAGER->GetNetConfig( Net::ServerType::Master ) );
    netServer_->ConnectAll();

    //Subscribe Events
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( NetworkHandler, HandleMessage ) );

    //Global Validation
    AddValidation( MESSAGE_HANDLER( NetworkHandler, CanProcessMessage ) );

    //Handlers
    Handle(MSGID_SelectCharacter ).Process( MESSAGE_HANDLER( UserHandler, HandleSelectCharacter ) );
    Handle( MSGID_CreateCharacter ).Process( MESSAGE_HANDLER( UserHandler, HandleCreateCharacter ) );

    return true;
}

void NetworkHandler::UnInit()
{
    netServer_->UnInit();
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