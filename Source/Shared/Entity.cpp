#include "PrecompiledHeader.h"
#include "Entity.h"

namespace Core
{
Entity::Entity( Context* context ) : 
    LogicComponent( context ),
    EntityCollision(),
    EntityNavigation(),
    animationMgr_( nullptr )
{
}

Entity::~Entity()
{
    animationMgr_ = nullptr;

    if( node_ )
        node_->Remove();
}

void Entity::Start()
{
}

void Entity::FixedUpdate( float time )
{
    HandleNavigationNode();
}

void Entity::FixedPostUpdate( float time )
{
    HandleCollisionNode();
}
};
