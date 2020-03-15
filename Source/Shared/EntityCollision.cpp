#include "PrecompiledHeader.h"
#include "EntityCollision.h"

#include "Entity.h"

#undef new

#include <Bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>

namespace Core
{
void EntityCollision::CreatePhysicsComponent( float diameter_, float height_ )
{
    //Set new diameter and height
    diameter = diameter_;
    height = height_;

    auto entity = static_cast<Entity*>(this);

    //Initial Node Transform
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin( btVector3( entity->node_->GetWorldPosition().x_, entity->node_->GetWorldPosition().y_, entity->node_->GetWorldPosition().z_ ) );
    startTransform.setRotation( ToBtQuaternion( Quaternion( 90, 0, 0 ) ) );

    //Capsule used for collision
    btConvexShape* capsule = new btCapsuleShape( diameter * 0.5f, height - diameter );

    //Getting Physics World from Scene
    btDiscreteDynamicsWorld* physicsWorld = entity->GetScene()->GetComponent<PhysicsWorld>()->GetWorld();

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

void EntityCollision::HandleCollisionNode()
{
    auto entity = static_cast<Entity*>(this);

    if( ghostObject )
    {
        currentCollisions.Clear();

        btManifoldArray manifoldArray;

        //Process all current collision events
        //(Ask ghost shape for a list of objects that it is potentially "colliding" with)
        int numObjects = ghostObject->getNumOverlappingObjects();

        for( int i = 0; i < numObjects; i++ )
        {
            manifoldArray.clear();

            //Access the next collision object whose AABB overlaps with that of our ghost shape
            btCollisionObject* obj = ghostObject->getOverlappingObject( i );

            //Try to cast the current collision object to bullet rigidbody
            //If this fails, its not a rigidbody - could be another ghost etc.
            btRigidBody* rb = dynamic_cast<btRigidBody*>(obj);
            if( rb )
            {
                //Query the physics broadphase for deeper information about the colliding pair
                auto* paircache = entity->node_->GetScene()->GetComponent<PhysicsWorld>()->GetWorld()->getPairCache();
                btBroadphasePair* collisionPair = paircache->findPair( ghostObject->getBroadphaseHandle(), obj->getBroadphaseHandle() );

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
                {
                    btPersistentManifold* manifold = manifoldArray[i];
                    numContacts += manifold->getNumContacts();
                }

                if( numContacts == 0 )
                    continue;

                //Cast the bullet rigidbody userpointer to Urho RigidBody
                //Dangerous assumption that this can never fail - hope springs eternal!
                RigidBody* RB = (RigidBody*)rb->getUserPointer();

                //Wrap the object pointer
                WeakPtr<RigidBody> weakRB( RB );

                VariantMap newData{};

                //Determine if this collision is "new", or "persistant"
                if( !prevCollisions.Contains( weakRB ) )
                {
                    //Send "collision started" event
                    newData[GhostCollisionBegin::P_BODY] = RB;
                    newData[GhostCollisionBegin::P_GHOST] = ghostObject;
                    newData[GhostCollisionBegin::P_GHOSTNODE] = entity->node_;
                    RB->GetNode()->SendEvent( E_GHOST_COLLISION_STARTED, newData );

                    //Collect the new collision
                    currentCollisions.Insert( weakRB );
                }
                else
                {
                    //Send "collision ongoing" event
                    newData[GhostCollisionStay::P_BODY] = RB;
                    newData[GhostCollisionStay::P_GHOST] = ghostObject;
                    newData[GhostCollisionStay::P_GHOSTNODE] = entity->node_;
                    RB->GetNode()->SendEvent( E_GHOST_COLLISION_STAY, newData );
                }
            }
        }

        //Process any collisions which have ended
        for( auto it = prevCollisions.Begin(); it != prevCollisions.End(); it++ )
        {

            //Check that the object has not been destroyed, and that the collision has ceased
            if( (*it) != nullptr && !currentCollisions.Contains( *it ) )
            {
                VariantMap newData{};
                newData[GhostCollisionEnded::P_BODY] = *it;
                newData[GhostCollisionEnded::P_GHOST] = ghostObject;
                newData[GhostCollisionEnded::P_GHOSTNODE] = entity->node_;
                (*it)->GetNode()->SendEvent( E_GHOST_COLLISION_ENDED, newData );
            }
        }

        //Keep track of collisions across frames
        prevCollisions = currentCollisions;
    }
}
};