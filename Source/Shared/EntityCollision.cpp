#include "PrecompiledHeader.h"
#include "EntityCollision.h"

#include "Entity.h"

#undef new

#include <Bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>

namespace Core
{
void EntityCollision::CreatePhysicsComponent( float diameter, float height )
{
    //Set new diameter and height
    diameter_ = diameter;
    height_ = height;

    auto entity = static_cast<Entity*>(this);

    //Get Physics World from Scene
    if( btDiscreteDynamicsWorld* physicsWorld = entity->GetScene()->GetComponent<PhysicsWorld>()->GetWorld(); physicsWorld )
    {
        auto collisionShape = CreateCollisionShape( diameter * 0.5f, height - diameter );
        ghostObject_ = CreateGhostObject( entity->node_, collisionShape );
        bulletController_ = CreateCharacterController( entity->node_, collisionShape, ghostObject_ );

        physicsWorld->addCollisionObject( ghostObject_, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter );
        physicsWorld->addAction( bulletController_ );
    }
}

void EntityCollision::HandleCollisionNode()
{
    auto entity = static_cast<Entity*>(this);

    if( ghostObject_ )
    {
        currentCollisions_.Clear();
        btManifoldArray manifoldArray;

        //Process all current collision events
        //(Ask ghost shape for a list of objects that it is potentially "colliding" with)
        int numObjects = ghostObject_->getNumOverlappingObjects();

        for( int i = 0; i < numObjects; i++ )
        {
            manifoldArray.clear();

            //Access the next collision object whose AABB overlaps with that of our ghost shape
            btCollisionObject* obj = ghostObject_->getOverlappingObject( i );

            //Try to cast the current collision object to bullet rigidbody
            //If this fails, its not a rigidbody - could be another ghost etc.
            btRigidBody* rb = dynamic_cast<btRigidBody*>(obj);
            if( rb )
            {
                //Query the physics broadphase for deeper information about the colliding pair
                auto* paircache = entity->node_->GetScene()->GetComponent<PhysicsWorld>()->GetWorld()->getPairCache();
                btBroadphasePair* collisionPair = paircache->findPair( ghostObject_->getBroadphaseHandle(), obj->getBroadphaseHandle() );

                if( collisionPair == nullptr )
                    continue;

                //Query the colliding pair for deeper information about the contact manifold(s)
                if( collisionPair->m_algorithm != nullptr )
                    collisionPair->m_algorithm->getAllContactManifolds( manifoldArray );

                if( manifoldArray.size() == 0 )
                    continue;

                //Confirm that the two objects are in contact
                int numContacts = 0;
                for( int i = 0; i < manifoldArray.size(); i++ )
                    numContacts += manifoldArray[i]->getNumContacts();

                if( numContacts == 0 )
                    continue;

                //Cast the bullet rigidbody userpointer to Urho RigidBody
                //Dangerous assumption that this can never fail - hope springs eternal!
                RigidBody* RB = (RigidBody*)rb->getUserPointer();

                //Wrap the object pointer
                WeakPtr<RigidBody> weakRB( RB );

                //Determine if this collision is "new", or "persistant"
                if( !prevCollisions_.Contains( weakRB ) )
                {
                    //Send "collision started" event
                    SendCollisionEvent( E_GHOST_COLLISION_STARTED, RB, entity->node_ );
                    currentCollisions_.Insert( weakRB );
                }
                else
                    SendCollisionEvent( E_GHOST_COLLISION_STAY, RB, entity->node_ );
            }
        }

        //Process any collisions which have ended
        for( const auto& previousCollisionObject : prevCollisions_ )
            if( previousCollisionObject && currentCollisions_.Contains( previousCollisionObject ) == false )
                SendCollisionEvent( E_GHOST_COLLISION_ENDED, previousCollisionObject, entity->node_ );

        //Keep track of collisions across frames
        prevCollisions_ = currentCollisions_;
    }
}

inline void EntityCollision::SendCollisionEvent( StringHash event, RigidBody* body, Node* node )
{
    VariantMap newData{};
    newData[GhostCollisionEnded::P_BODY] = body;
    newData[GhostCollisionEnded::P_GHOST] = ghostObject_;
    newData[GhostCollisionEnded::P_GHOSTNODE] = node;
    body->GetNode()->SendEvent( event, newData );
}

inline const btTransform EntityCollision::GetInitialTransform( Node* node ) const
{
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin( btVector3( node->GetWorldPosition().x_, node->GetWorldPosition().y_, node->GetWorldPosition().z_ ) );
    startTransform.setRotation( ToBtQuaternion( Quaternion( 90, 0, 0 ) ) );
    return startTransform;
}

inline btConvexShape* EntityCollision::CreateCollisionShape( float radius, float height )
{
    return new btCapsuleShape( radius, height );
}

inline btPairCachingGhostObject* EntityCollision::CreateGhostObject( Node* node, btCollisionShape* collisionShape ) const
{
    auto physicsWorld = node->GetScene()->GetComponent<PhysicsWorld>()->GetWorld();
    auto collisionObject = new btPairCachingGhostObject();

    if( collisionObject && physicsWorld )
    {
        collisionObject->setWorldTransform( GetInitialTransform( node ) );
        physicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );
        collisionObject->setCollisionShape( collisionShape );
        collisionObject->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
        return collisionObject;
    }

    return nullptr;
}

inline btKinematicCharacterController* EntityCollision::CreateCharacterController( Node* node, btConvexShape* collisionShape, btPairCachingGhostObject* collisionObject )
{
    auto physicsWorld = node->GetScene()->GetComponent<PhysicsWorld>()->GetWorld();
    
    if( node && collisionObject )
    {
        auto characterController = new btKinematicCharacterController( collisionObject, collisionShape, 0.3f, btVector3( 0, 0, 1 ) );
        characterController->setGravity( physicsWorld->getGravity() );
        characterController->setMaxJumpHeight( 1.5 );
        return characterController;
    }

    return nullptr;
}
};