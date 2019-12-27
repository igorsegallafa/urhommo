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
    height( 2.3f ),
    diameter( 0.7f ),
    ghostObject( nullptr ),
    bulletController( nullptr )
{
}

Entity::~Entity()
{
    if( node_ )
        node_->Remove();
}

void Entity::Start()
{
}

void Entity::CreatePhysicsComponent()
{
    //Initial Node Transform
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin( btVector3( node_->GetWorldPosition().x_, node_->GetWorldPosition().y_, node_->GetWorldPosition().z_ ) );
    startTransform.setRotation( ToBtQuaternion( Quaternion( 90, 0, 0 ) ) );

    //Capsule used for collision
    btConvexShape* capsule = new btCapsuleShape( diameter * 0.5f, height - diameter );

    //Getting Physics World from Scene
    btDiscreteDynamicsWorld* physicsWorld = GetScene()->GetComponent<PhysicsWorld>()->GetWorld();

    if( physicsWorld )
    {
        ghostObject = new btPairCachingGhostObject();
        ghostObject->setWorldTransform( startTransform );
        physicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );
        ghostObject->setCollisionShape( capsule );
        ghostObject->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
        bulletController = new btKinematicCharacterController( ghostObject, capsule, 0.3f, btVector3( 0, 0, 1 ) );
        bulletController->setGravity( physicsWorld->getGravity() );

        physicsWorld->addCollisionObject( ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter );
        physicsWorld->addAction( bulletController );
        bulletController->setMaxJumpHeight( 1.5 );
    }
}

};
