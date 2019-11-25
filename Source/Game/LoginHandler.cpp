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
        VariantMap t;
        t[Shared::Login::P_ACCOUNTNAME] = account;
        t[Shared::Login::P_PASSWORD] = password;
        NETWORKHANDLER->ConnectLoginServer( "127.0.0.1", 52011, t );
    }
}
}
