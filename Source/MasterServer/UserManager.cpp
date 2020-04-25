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

SharedPtr<User> UserManager::AddUser( Connection* connection )
{
    //Doesn't add net connection as user
    if( connection->IsNetConnection() )
        return nullptr;

    auto it = users_.Find( connection );

    //We can't hold two instances of user for the same connection
    if( it != users_.End() )
        return it->second_;

    //User Added
    SharedPtr<User> newUser( new User( context_ ) );
	newUser->connection_ = connection;
    users_[connection] = newUser;

    return newUser;
}

void UserManager::DelUser( Connection* connection )
{
    auto it = users_.Find( connection );

    if( it != users_.End() )
        users_.Erase( it );
}