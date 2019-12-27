#include "PrecompiledHeader.h"
#include "UserHandler.h"

UserHandler::UserHandler( Context* context ) :
    HandlerImpl( context )
{
}

UserHandler::~UserHandler()
{
}

bool UserHandler::HandleWorldData( Connection* connection, MemoryBuffer & message )
{
    auto user = USERMANAGER->GetUser( connection );
    if( user )
    {
        //Set User Scene
        auto map = MAPMANAGER->GetMap( user->mapID );

        if( map )
            connection->SetScene( map->scene );

        return true;
    }

    return false;
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
                user->mapID = mapID;

                //Load Character
                return LoadCharacter( user, mapID, position );
            }

            break;
        }
    }

    return false;
}

bool UserHandler::LoadCharacter( User* user, const MapID& mapID, const Vector3& position )
{
    auto map = MAPMANAGER->GetMap( mapID );

    if( map && user )
    {
        auto xmlFile = RESOURCECACHE->GetResource<XMLFile>( "Objects/Character.xml" );
        auto characterNode = map->scene->InstantiateXML( xmlFile->GetRoot(), position, Quaternion::IDENTITY );

        //Set the Connection Owner
        characterNode->SetOwner( user->connection );

        //Create Character Component
        user->character = characterNode->GetOrCreateComponent<Core::Character>();
        user->character->connection = user->connection;
        characterNode->GetOrCreateComponent<SmoothedTransform>();

        //Set Character Name
        characterNode->GetChild( "HeadNode" )->GetComponent<Text3D>()->SetText( user->characterName );

        //Send Character Node ID for Client
        VectorBuffer worldDataMsg;
        worldDataMsg.WriteInt( characterNode->GetID() );
        worldDataMsg.WriteInt( (MAP_ID)mapID );
        user->connection->Send( MSGID_WorldData, true, true, worldDataMsg );

        return true;
    }

    return false;
}
