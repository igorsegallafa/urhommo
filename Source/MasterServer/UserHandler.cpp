#include "PrecompiledHeader.h"
#include "UserHandler.h"

UserHandler::UserHandler( Context* context ) :
    HandlerImpl( context )
{
}

UserHandler::~UserHandler()
{
}

bool UserHandler::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_CLIENTCONNECTED, URHO3D_HANDLER( UserHandler, HandleUserConnected ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( UserHandler, HandleUserDisconnected ) );

    return true;
}

bool UserHandler::HandleLoadUser( Connection* connection, MemoryBuffer& message ) //@Net::MSGID_LoadUser
{
    auto address = message.ReadString();
    auto port = message.ReadInt();

    //Find User by IP and Port
    for( const auto& clientConnection : NETWORK->GetClientConnections() )
    {
        if( clientConnection->GetAddress() == address && clientConnection->GetPort() == port )
        {
            auto user = USERMANAGER->GetUser( clientConnection );
            
            //Load User
            if( user )
            {
                auto accountName = message.ReadString();
                auto characterName = message.ReadString();

                user->accountName_ = accountName;
                user->characterName_ = characterName;

                //Send message for Game Server
                NETSERVER->Send( Net::ServerType::World, Net::MSGID_LoadUser, true, true, VectorBuffer( message.GetData(), message.GetSize() ) );
            }

            break;
        }
    }

    return true;
}

void UserHandler::HandleUserConnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientConnected::P_CONNECTION].GetPtr());

    if( connection )
        USERMANAGER->AddUser( connection );
}

void UserHandler::HandleUserDisconnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientDisconnected::P_CONNECTION].GetPtr());

    if( connection )
        USERMANAGER->DelUser( connection );
}
