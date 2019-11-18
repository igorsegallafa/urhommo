#include "PrecompiledHeader.h"
#include "User.h"

namespace Shared::Core
{
User::User( Context* context_ ) : 
    Object( context_ ),
    connection( nullptr )
{
}

User::~User()
{
    connection = nullptr;
}
}