#include "PrecompiledHeader.h"
#include "User.h"

namespace Shared::Core
{
User::User( Context* context ) :
    Object( context ),
    connection( nullptr )
{
}

User::~User()
{
    connection = nullptr;
}
}