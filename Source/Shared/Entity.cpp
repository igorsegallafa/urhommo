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
        SubscribeToEvent( E_POSTRENDERUPDATE, URHO3D_HANDLER( Entity, HandlePostRenderUpdate ) );
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
    if( targetPath.Size() )
    {
        Vector3 nextWaypoint = targetPath[0];
        float distance = (Vector3(node_->GetPosition().x_,0.f, node_->GetPosition().z_) - Vector3(nextWaypoint.x_,0.f, nextWaypoint.z_)).Length();
        Vector3 dir = nextWaypoint - node_->GetPosition();

        dir.y_ = 0.f;
        targetDirection = dir.Normalized();
        followingTarget = true;

        //Remove waypoint if reached it
        if( distance < 0.1f )
            targetPath.Erase( 0 );
    }
    else
        followingTarget = false;
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
        targetPos = dest;

        auto scene = node_->GetScene();
        auto navigationMesh = scene->GetComponent<NavigationMesh>( true );

        if( navigationMesh )
            navigationMesh->FindPath( targetPath, node_->GetWorldPosition(), dest, Vector3( 10.f, 10.f, 10.f ) );
    }
}

void Entity::ResetTargetPosition()
{
    if( node_ )
    {
        followingTarget = false;
        targetDirection = Vector3::ZERO;
        targetPath.Clear();
    }
}

void Entity::HandlePostRenderUpdate( StringHash eventType, VariantMap & eventData )
{
    if( targetPath.Size() )
    {
        //Visualize the current calculated path
        auto* debug = node_->GetScene()->GetComponent<DebugRenderer>();
        debug->AddBoundingBox( BoundingBox( targetPos - Vector3( 0.1f, 0.1f, 0.1f ), targetPos + Vector3( 0.1f, 0.1f, 0.1f ) ), Color( 1.0f, 1.0f, 1.0f ) );

        //Draw the path with a small upward bias so that it does not clip into the surfaces
        Vector3 bias( 0.0f, 0.05f, 0.0f );
        debug->AddLine( node_->GetPosition() + bias, targetPath[0] + bias, Color( 1.0f, 1.0f, 1.0f ) );

        if( targetPath.Size() > 1 )
        {
            for( unsigned i = 0; i < targetPath.Size() - 1; ++i )
                debug->AddLine( targetPath[i] + bias, targetPath[i + 1] + bias, Color( 1.0f, 1.0f, 1.0f ) );
        }
    }
}
};
