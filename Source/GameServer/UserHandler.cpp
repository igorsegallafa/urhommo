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

bool UserHandler::HandleWorldData( Connection* connection, MemoryBuffer& message ) //@MSGID_WorldData
{
    auto user = USERMANAGER->GetUser( connection );
    if( user )
    {
        //Set User Scene
        auto map = MAPMANAGER->GetMap( user->mapID_ );

        if( map )
            connection->SetScene( MAPMANAGER->GetScene() );

        return true;
    }

    return false;
}

bool UserHandler::HandleLoadUser( Connection* connection, MemoryBuffer& message ) //@Net::MSGID_LoadUser
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
                user->accountName_ = accountName;
                user->characterName_ = characterName;
                user->characterClass_ = static_cast<CharacterClass>(characterClass);
                user->mapID_ = mapID;

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
        auto characterNode = MAPMANAGER->GetScene()->InstantiateXML( xmlFile->GetRoot(), position, Quaternion::IDENTITY );

        //Set the Connection Owner
        characterNode->SetOwner( user->connection_ );

        context_->RegisterFactory<Core::CharacterEquipment>();

        //Create Character Component
        user->character_ = characterNode->GetOrCreateComponent<Core::Character>();
        user->character_->connection_ = user->connection_;
        user->character_->animationMgr_ = characterNode->GetOrCreateComponent<Core::AnimationEntity>();
        user->character_->equipmentMgr_ = characterNode->GetOrCreateComponent<Core::CharacterEquipment>( LOCAL );
        characterNode->GetOrCreateComponent<SmoothedTransform>();

        if( auto characterNameNode = characterNode->GetChild( "Name" ); characterNameNode )
            characterNameNode->GetComponent<Text3D>()->SetText( user->characterName_ );

        //Set Default Equipment
        user->character_->equipmentMgr_->SetHead( EQUIPMENTMANAGER->GetEquipModelData( EquipType::Head, user->characterClass_, 0 ) );
        user->character_->equipmentMgr_->SetArmor( EQUIPMENTMANAGER->GetEquipModelData( EquipType::Armor, user->characterClass_, 0 ) );

        //Load Animation Set
        user->character_->animationMgr_->Load( "Definitions/Animations/fighter.json" );

        //Set Connection Position
        user->connection_->SetPosition( Vector3( -1.f, -1.f, -1.f ) );

        //Send Character Node ID for Client
        VectorBuffer worldDataMsg;
        worldDataMsg.WriteInt( characterNode->GetID() );
        worldDataMsg.WriteInt( (MAP_ID)mapID );
        user->connection_->Send( MSGID_WorldData, true, true, worldDataMsg );
        return true;
    }

    return false;
}

void UserHandler::HandleUserConnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientConnected::P_CONNECTION].GetPtr());

    if( connection )
    {
        //Add User to Server Memory
        USERMANAGER->AddUser( connection );

        //Send the message to confirm the game server was connected
        connection->Send( MSGID_GameServerConnected, true, true, VectorBuffer() );
    }
}

void UserHandler::HandleUserDisconnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientDisconnected::P_CONNECTION].GetPtr());

    if( connection )
        USERMANAGER->DelUser( connection );
}