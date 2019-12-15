#include "PrecompiledHeader.h"
#include "UserHandler.h"

UserHandler::UserHandler( Context* context ) :
    HandlerImpl( context )
{
}

UserHandler::~UserHandler()
{
}

bool UserHandler::HandleLoadUser( Connection* connection, MemoryBuffer& message )
{
    auto address = message.ReadString();
    auto port = message.ReadInt();

    //Find User by IP::Port
    for( const auto& clientConnection : NETWORK->GetClientConnections() )
    {
        //Found!
        if( clientConnection->GetAddress().Compare( address ) == 0 && clientConnection->GetPort() == port )
        {
            auto user = USERMANAGER->GetUser( clientConnection );
            
            //Load User
            if( user )
            {
                auto accountName = message.ReadString();
                auto characterName = message.ReadString();

                //Set Account Name and Character Name for User
                user->SetAccountName( accountName );
                user->SetCharacterName( characterName );
            }

            break;
        }
    }

    return true;
}
