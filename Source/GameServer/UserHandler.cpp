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
                auto characterClass = message.ReadInt();
                auto mapID = (MapID)message.ReadInt();
                auto position = message.ReadVector3();

                //Set Account Name and Character Name for User
                user->accountName = accountName;
                user->characterName = characterName;

                //Set User Scene
                auto map = MAPMANAGER->GetMap( mapID );
                if( map )
                    clientConnection->SetScene( map->scene );
            }

            break;
        }
    }

    return true;
}
