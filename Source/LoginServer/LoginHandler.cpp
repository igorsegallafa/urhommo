#include "PrecompiledHeader.h"
#include "LoginHandler.h"

namespace Handler
{
Login::Login( Context* context ) : 
    Impl( context )
{
}

Login::~Login()
{
}

bool Login::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( Login, HandleClientIdentity ) );

    return true;
}

void Login::ProcessLogin( Core::User* user )
{
    VectorBuffer message;

    //Write Total of Game Servers
    message.WriteInt( 1 );

    //Write Game Servers Info
    for( int i = 0; i < 1; i++ )
    {
        message.WriteString( "Beta" );
        message.WriteString( "127.0.0.1" );
        message.WriteInt( 52012 );
    }

    //Write Character List
    {

    }

    //Send Message
    user->GetConnection()->Send( Shared::Network::MSGID_LoginData, true, true, message );
}

void Login::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
    if( Variant outAccountName, outPassword; eventData.TryGetValue( Shared::Login::P_ACCOUNTNAME, outAccountName ) && eventData.TryGetValue( Shared::Login::P_PASSWORD, outPassword ) )
    {
        auto connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());

        //Process Account Login
        //eventData[ClientIdentity::P_ALLOW] = false;

        //Add User to Server Memory
        auto user = USERMANAGER->AddUser( connection );

        //User Allocated?
        if( user )
        {
            //Process Login of User
            ProcessLogin( user );
        }
    }
}
}