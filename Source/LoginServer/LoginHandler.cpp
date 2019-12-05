#include "PrecompiledHeader.h"
#include "LoginHandler.h"

LoginHandler::LoginHandler( Context* context ) :
    HandlerImpl( context )
{
}

LoginHandler::~LoginHandler()
{
}

bool LoginHandler::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( LoginHandler, HandleClientIdentity ) );

    return true;
}

void LoginHandler::ProcessLogin( Core::User* user )
{
    VectorBuffer message;
    unsigned int totalGameServers = 0;

    //Get Total Game Servers
    for( const auto& gameServer : CONFIGMANAGER->GetNetConnections() )
        if( gameServer->serverType == Net::ServerType::Game )
            totalGameServers++;

    //Write Total of Game Servers
    message.WriteInt( totalGameServers );

    //Write Game Servers Info
    for( unsigned int i = 0; i < totalGameServers; i++ )
    {
        auto serverConfig = CONFIGMANAGER->GetNetConfig( Net::ServerType::Game, i );
        message.WriteString( serverConfig->name );
        message.WriteString( serverConfig->ip );
        message.WriteInt( serverConfig->port );
    }

    //Write Character List
    {

    }

    //Send Message
    user->GetConnection()->Send( MSGID_LoginData, true, true, message );
}

void LoginHandler::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
    if( Variant outAccountName, outPassword; eventData.TryGetValue( Login::P_ACCOUNTNAME, outAccountName ) && eventData.TryGetValue( Login::P_PASSWORD, outPassword ) )
    {
        auto connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());

        //TODO: Process Account Login
        //eventData[ClientIdentity::P_ALLOW] = false;

        //Add User to Server Memory
        auto user = USERMANAGER->AddUser( connection );

        //User Allocated?
        if( user )
        {
            //Set AccountName
            user->SetAccountName( outAccountName.GetString() );

            //Process Login of User
            ProcessLogin( user );
        }
    }
}
