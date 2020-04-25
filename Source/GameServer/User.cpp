#include "PrecompiledHeader.h"
#include "User.h"

User::User( Context* context ) :
    Core::User( context ),
    character_( nullptr ),
    characterClass_( CharacterClass::Undefined )
{

}

User::~User()
{
    if( (character_) && character_->GetNode() )
        character_->GetNode()->Remove();
}