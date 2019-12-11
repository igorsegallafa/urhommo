#include "PrecompiledHeader.h"
#include "Entity.h"

namespace Core
{
Entity::Entity( Context* context ) : 
    LogicComponent( context ),
    height( 2.3f ),
    diameter( 0.7f ),
    ghostObject( nullptr ),
    bulletController( nullptr )
{
}

Entity::~Entity()
{
}

};
