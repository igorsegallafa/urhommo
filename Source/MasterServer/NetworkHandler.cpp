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
    netServer_->ConnectAll();

    //Subscribe Events
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( NetworkHandler, HandleMessage ) );

    //Global Validation
    AddValidation( MESSAGE_HANDLER( NetworkHandler, CanProcessMessage ) );

    //Net Messages
    Handle( Net::MSGID_LoadUser ).Process( MESSAGE_HANDLER( UserHandler, HandleLoadUser ) );

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
        return false;

    return true;
}