#include "PrecompiledHeader.h"
#include "UserManager.h"

UserManager::UserManager( Context* context ) : ManagerImpl( context )
{
}

UserManager::~UserManager()
{
}

bool UserManager::Init()
{
    return true;
}

void UserManager::UnInit()
{
}

SharedPtr<Core::User> UserManager::AddUser( Connection* connection )
{
    auto it = users.Find( connection );

    //We can't hold two instances of user for the same connection
    if( it != users.End() )
        return it->second_;

    //User Added
    SharedPtr<Core::User> newUser( new Core::User( context_ ) );
    newUser->SetConnection( connection );
    users[connection] = newUser;

    return newUser;
}

void UserManager::DelUser( Connection* connection )
{
    auto it = users.Find( connection );

    if( it != users.End() )
        users.Erase( it );
}