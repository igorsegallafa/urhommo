#include "PrecompiledHeader.h"
#include "LoginHandler.h"

LoginHandler::LoginHandler( Context* context ) :
    HandlerImpl( context )
{
}

LoginHandler::~LoginHandler()
{
}

void LoginHandler::ProcessLogin( const String& account, const String& password )
{
    if( account.Length() && password.Length() )
    {
        VariantMap identity;
        identity[Login::P_ACCOUNTNAME] = account;
        identity[Login::P_PASSWORD] = password;
        NETWORKHANDLER->ConnectLoginServer( "127.0.0.1", 52011, identity );
    }
}

bool LoginHandler::HandleLoginData( Connection* connection, MemoryBuffer& message )
{
    Beep( 300, 300 );
    return true;
}