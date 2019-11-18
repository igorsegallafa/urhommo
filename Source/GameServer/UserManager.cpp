#include "PrecompiledHeader.h"
#include "UserManager.h"

namespace Manager
{
User::User( Context* context_ ) : Impl( context_ )
{
}

User::~User()
{
}

bool User::Init()
{
    return true;
}

void User::UnInit()
{
}

SharedPtr<Core::User> User::AddUser( Connection* connection )
{
    auto it = users.Find( connection );

    //We can't hold two instances of user for the same connection
    if( it != users.End() )
        return it->second_;

    //User Added
    SharedPtr<Core::User> newUser( new Core::User( context_ ) );
    users[connection] = newUser;

    return newUser;
}

void User::DelUser( Connection* connection )
{
    auto it = users.Find( connection );

    if( it != users.End() )
        users.Erase( it );
}
}