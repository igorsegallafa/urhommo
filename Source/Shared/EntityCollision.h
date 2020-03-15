#pragma once

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace Urho3D{ class RigidBody; };

URHO3D_EVENT( E_GHOST_COLLISION_STARTED, GhostCollisionBegin )
{
    URHO3D_PARAM( P_BODY, Body );             /// RigidBody which collided with btGhostObject
    URHO3D_PARAM( P_GHOST, Ghost );           /// btGhostObject which collided with RigidBody
    URHO3D_PARAM( P_GHOSTNODE, GhostNode );   /// Scene node which acts as Parent to btGhostNode
}

URHO3D_EVENT( E_GHOST_COLLISION_STAY, GhostCollisionStay )
{
    URHO3D_PARAM( P_BODY, Body );
    URHO3D_PARAM( P_GHOST, Ghost );
    URHO3D_PARAM( P_GHOSTNODE, GhostNode );
}

URHO3D_EVENT( E_GHOST_COLLISION_ENDED, GhostCollisionEnded )
{
    URHO3D_PARAM( P_BODY, Body );
    URHO3D_PARAM( P_GHOST, Ghost );
    URHO3D_PARAM( P_GHOSTNODE, GhostNode );
}

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