#pragma once

#include "CollisionDef.h"

class btPairCachingGhostObject;
class btKinematicCharacterController;
class btTransform;
class btCollisionShape;
class btConvexShape;

namespace Urho3D{ class RigidBody; };

namespace Core
{
class EntityCollision
{
public:
    //! Constructor.
    EntityCollision() : 
        ghostObject_( nullptr ), 
        bulletController_( nullptr ),
        diameter_( 0.f ),
        height_( 0.f )
    {
    }

    //! Deconstructor.
    ~EntityCollision() { ghostObject_ = nullptr; bulletController_ = nullptr; }

    //! Create Physics Component.
    void CreatePhysicsComponent( float diameter = 0.7f, float height = 2.3f );

    //! Handle Collision Node.
    void HandleCollisionNode();
private:
    inline void SendCollisionEvent( StringHash event, RigidBody* body, Node* node );

    inline const btTransform GetInitialTransform( Node* node ) const;
    inline btConvexShape* CreateCollisionShape( float radius, float height );
    inline btPairCachingGhostObject* CreateGhostObject( Node* node, btCollisionShape* collisionShape ) const;
    inline btKinematicCharacterController* CreateCharacterController( Node* node, btConvexShape* collisionShape, btPairCachingGhostObject* collisionObject );
protected:
    float diameter_;
    float height_;

    btPairCachingGhostObject* ghostObject_;
    btKinematicCharacterController* bulletController_;

    HashSet<WeakPtr<RigidBody>> prevCollisions_;
    HashSet<WeakPtr<RigidBody>> currentCollisions_;
};
};