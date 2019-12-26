#include "PrecompiledHeader.h"
#include "User.h"

User::User( Context* context ) :
    Core::User( context ),
    character( nullptr )
{

}

User::~User()
{
    if( (character) && character->GetNode() )
        character->GetNode()->Remove();
}