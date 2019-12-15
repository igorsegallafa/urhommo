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
        if( clientConnection->GetAddress() == address && clientConnection->GetPort() == port )
        {
            auto user = USERMANAGER->GetUser( clientConnection );
            
            //Load User
            if( user )
            {
                auto accountName = message.ReadString();
                auto characterName = message.ReadString();

                user->accountName = accountName;
                user->characterName = characterName;
            }

            break;
        }
    }

    return true;
}
