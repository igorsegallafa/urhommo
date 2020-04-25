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
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( UserHandler, HandleUserValidation ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( UserHandler, HandleUserDisconnected ) );

    return true;
}

void UserHandler::HandleUserValidation( StringHash eventType, VariantMap& eventData )
{
    if( Variant outAccountName, outPassword; eventData.TryGetValue( Login::P_ACCOUNTNAME, outAccountName ) && eventData.TryGetValue( Login::P_PASSWORD, outPassword ) )
    {
        int outUserID = -1;

        auto loginStatus = LOGINHANDLER->HandleUserValidation(outAccountName.GetString(), outPassword.GetString(), outUserID );
        auto connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());

        //Add User to Server Memory
        auto user = USERMANAGER->AddUser( connection );

        //User Allocated?
        if( user )
        {
            //Set User ID
            user->id_ = outUserID;

            //Set AccountName
            user->accountName_ = outAccountName.GetString();

            //Process User Login
            if( loginStatus == LoginStatus::None )
                loginStatus |= LOGINHANDLER->HandleUserLogin( user );

            //Process Login Response Message
            LOGINHANDLER->ProcessLoginResponse( loginStatus, user );

            //Successful Login
            if( loginStatus & LoginStatus::Successful )
            {
            }
            else
                user->connection_->Disconnect( 50 );
        }
    }
}

void UserHandler::HandleUserDisconnected( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());

    if( connection )
        USERMANAGER->DelUser( connection );
}

bool UserHandler::HandleSelectCharacter( Connection* connection, MemoryBuffer& message ) //@MSGID_SelectCharacter
{
    auto user = USERMANAGER->GetUser( connection );
    auto characterName = message.ReadString();
    auto db = DATABASEMANAGER->Get( DatabaseConn::AoR );

    if( db && user )
    {
        //Create Character into Database
        String query =  "SELECT level, class "
                        "FROM aor_user.characters "
                        "WHERE user_id = $1 AND name = $2 LIMIT 1;";

        auto results = db->Exec( query, user->id_, characterName.CString() );

        if( auto result = results[0]; results.size() )
        {
            //Process World Data for Client
            ProcessWorldData( connection, characterName, static_cast<CharacterClass>(result["class"].as<int>()), result["level"].as<int>(), MapID::Ricarten, Vector3( Random( -76.9475f, -73.9475f ), 5.0f, -22.1408f ) );
            return true;
        }
    }

    return false;
}

bool UserHandler::HandleCreateCharacter( Connection* connection, MemoryBuffer& message ) //@MSGID_CreateCharacter
{
    auto user = USERMANAGER->GetUser( connection );
    auto characterName = message.ReadString();
    auto characterClass = message.ReadInt();
    auto db = DATABASEMANAGER->Get( DatabaseConn::AoR );

    if( db && user )
    {
        //Create Character into Database
        String query =  "INSERT INTO aor_user.characters "
                        "(user_id, name, level, class, created_at) "
                        "VALUES ($1, $2, $3, $4, NOW());";

        auto results = db->Exec( query, user->id_, characterName.CString(), 1, characterClass );

        if( results.affected_rows() > 0 )
        {
            //Process World Data for Client
            ProcessWorldData( connection, characterName, static_cast<CharacterClass>(characterClass), 1, MapID::Ricarten, Vector3( Random( -76.9475f, -73.9475f ), 5.0f, -22.1408f ) );
            return true;
        }
    }

    return false;
}

void UserHandler::ProcessWorldData( Connection* connection, const String& characterName, const CharacterClass& characterClass,
                                    int characterLevel, const MapID& mapID, const Vector3& position )
{
    //TODO: Input Validation

    if( auto user = USERMANAGER->GetUser( connection ); user )
    {
        VectorBuffer loadUserMsg;
        loadUserMsg.WriteString( connection->GetAddress() );
        loadUserMsg.WriteInt( connection->GetPort() );
        loadUserMsg.WriteString( user->accountName_ );
        loadUserMsg.WriteString( characterName );
        loadUserMsg.WriteInt( (int)characterClass );
        loadUserMsg.WriteInt( (int)mapID );
        loadUserMsg.WriteVector3( position );

        //We can disconnect User from Login Server
        connection->Disconnect( 50 );

        NETSERVER->Send( Net::ServerType::Master, Net::MSGID_LoadUser, true, true, loadUserMsg );
    }
}
