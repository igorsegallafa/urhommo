#include "PrecompiledHeader.h"
#include "User.h"

namespace Core
{
User::User( Context* context ) :
    Object( context ),
    connection_( nullptr ),
    id_( -1 )
{
}

User::~User()
{
    connection_ = nullptr;
}
}