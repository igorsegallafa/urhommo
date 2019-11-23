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
        NETWORKHANDLER->ConnectLoginServer( "127.0.0.1", 52010, t );
        NETWORKHANDLER->ConnectMasterServer( "127.0.0.1", 52011, t );
        NETWORKHANDLER->ConnectGameServer( "127.0.0.1", 52012, t );
    }
}
}
