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

void Login::ProcessLogin( const String& account, const String& password )
{
    if( account.Length() && password.Length() )
    {
        VariantMap identity;
        identity[Shared::Login::P_ACCOUNTNAME] = account;
        identity[Shared::Login::P_PASSWORD] = password;
        NETWORKHANDLER->ConnectLoginServer( "127.0.0.1", 52011, identity );
    }
}

bool Login::HandleLoginData( Connection* connection, MemoryBuffer& message )
{
    Beep( 300, 300 );
    return true;
}
}
