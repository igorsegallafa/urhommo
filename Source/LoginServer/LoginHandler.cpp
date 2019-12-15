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
    VectorBuffer loginDataMsg;
    Core::LoginStatus loginStatus = Core::LoginStatus::Successful;
    unsigned int totalGameServers = 0;

    //Write Login Response Status
    loginDataMsg.WriteInt( (int)loginStatus );

    //Successful Login
    if( loginStatus == Core::LoginStatus::Successful )
    {
        //Get Total Game Servers
        for( const auto& gameServer : CONFIGMANAGER->GetNetConnections() )
            if( gameServer->serverType == Net::ServerType::Game )
                totalGameServers++;

        //Write Total of Game Servers
        loginDataMsg.WriteInt( totalGameServers );

        //Write Game Servers Info
        for( unsigned int i = 0; i < totalGameServers; i++ )
        {
            auto serverConfig = CONFIGMANAGER->GetNetConfig( Net::ServerType::Game, i );
            loginDataMsg.WriteString( serverConfig->name );
            loginDataMsg.WriteString( serverConfig->ip );
            loginDataMsg.WriteInt( serverConfig->port );
        }

        //Write Character List
        {
        }

        //Write Master Server Info
        auto masterServerInfo = CONFIGMANAGER->GetNetConfig( Net::ServerType::Master );
        loginDataMsg.WriteString( masterServerInfo->ip );
        loginDataMsg.WriteInt( masterServerInfo->port );
    }

    //Send Message
    user->connection->Send( MSGID_LoginData, true, true, loginDataMsg );

    //Disconnect if login has been failed
    if( loginStatus != Core::LoginStatus::Successful )
        user->connection->Disconnect( 50 );
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
            user->accountName = outAccountName.GetString();

            //Process Login of User
            ProcessLogin( user );
        }
    }
}
