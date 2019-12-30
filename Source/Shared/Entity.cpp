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
    bulletController( nullptr ),
    animationMgr( nullptr )
{
    if( Shared::IsGameRunning() )
        SubscribeToEvent( E_CROWD_AGENT_REPOSITION, URHO3D_HANDLER( Entity, HandleCrowdAgentReposition ) );
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

void Entity::SetTargetPosition( const Vector3& dest )
{
    //Reset Target Position
    ResetTargetPosition();

    //Valid node?
    if( node_ )
    {
        auto scene = node_->GetScene();
        auto crowdManager = scene->GetComponent<CrowdManager>();

        if( crowdManager )
            crowdManager->SetCrowdTarget( dest, node_ );
    }
}

void Entity::ResetTargetPosition()
{
    if( node_ )
    {
        auto scene = node_->GetScene();
        auto crowdManager = scene->GetComponent<CrowdManager>();

        if( crowdManager )
        {
            followingTarget = false;
            targetDirection = Vector3::ZERO;

            crowdManager->ResetCrowdTarget( node_ );
        }
    }
}

void Entity::HandleCrowdAgentReposition( StringHash eventType, VariantMap& eventData )
{
    auto crowdAgent = static_cast<CrowdAgent*>(eventData[CrowdAgentReposition::P_CROWD_AGENT].GetPtr());
    bool arrived = eventData[CrowdAgentReposition::P_ARRIVED].GetBool();

    if( crowdAgent )
    {
        Vector3 desiredVelocity = crowdAgent->GetDesiredVelocity();

        //Arrived to destiny? Reset Target Position
        if( arrived && followingTarget )
        {
            ResetTargetPosition();
            return;
        }

        //Validate Desired Velocity
        if( desiredVelocity != Vector3::ZERO )
        {
            targetDirection = desiredVelocity.Normalized();
            followingTarget = true;
        }
    }
}

};
