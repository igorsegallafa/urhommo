#pragma once

#include "CollisionDef.h"

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace Urho3D{ class RigidBody; };

namespace Core
{
class EntityCollision
{
public:
    //! Constructor.
    EntityCollision() : 
        ghostObject( nullptr ), 
        bulletController( nullptr ),
        diameter( 0.f ),
        height( 0.f )
    {
    }

    //! Deconstructor.
    ~EntityCollision() { ghostObject = nullptr; bulletController = nullptr; }

    //! Create Physics Component.
    void CreatePhysicsComponent( float diameter_ = 0.7f, float height_ = 2.3f );

    //! Handle Collision Node.
    void HandleCollisionNode();
protected:
    float diameter;
    float height;

    btPairCachingGhostObject* ghostObject;
    btKinematicCharacterController* bulletController;

    HashSet<WeakPtr<RigidBody>> prevCollisions;
    HashSet<WeakPtr<RigidBody>> currentCollisions;
};
};