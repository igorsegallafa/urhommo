#include "PrecompiledHeader.h"
#include "Entity.h"

#undef new

#include <Bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>

namespace Core
{
Entity::Entity( Context* context ) : 
    LogicComponent( context ),
    EntityCollision(),
    EntityNavigation(),
    animationMgr( nullptr )
{
}

Entity::~Entity()
{
    animationMgr = nullptr;

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
