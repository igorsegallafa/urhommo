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

void Login::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
    if( Variant outAccountName, outPassword; eventData.TryGetValue( Shared::Login::P_ACCOUNTNAME, outAccountName ) && eventData.TryGetValue( Shared::Login::P_PASSWORD, outPassword ) )
    {
        if( outAccountName.GetString().Compare( "123" ) == 0 && outPassword.GetString().Compare( "123" ) == 0 )
        {
            eventData[ClientIdentity::P_ALLOW] = false;
        }
    }
}
}