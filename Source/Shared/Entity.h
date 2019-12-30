#pragma once

#include "Animation.h"

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace Core
{
class Entity : public LogicComponent
{
    URHO3D_OBJECT( Entity, LogicComponent );
public:
    //! Default Constructor.
    Entity( Context* context );

    //! Deconstructor.
    ~Entity();

    //! Start Logic Component.
    void Start();

    //! Create Physics Component.
    void CreatePhysicsComponent();

    //! Set Target Position.
    void SetTargetPosition( const Vector3& dest );
    void ResetTargetPosition();

    //! Is Following Target Getter.
    bool IsFollowingTarget() const { return followingTarget; }

    //! Target Direction Getter and Setter.
    void SetTargetDirection( const Vector3& dir ) { targetDirection = dir; }
    const Vector3& GetTargetDirection() const { return targetDirection; }
private:
    //! Crowd Agent Reposition Handler.
    void HandleCrowdAgentReposition( StringHash eventType, VariantMap& eventData );
public:
    Core::AnimationEntity* animationMgr;
protected:
    float height;
    float diameter;

    bool followingTarget;
    Vector3 targetDirection;

    btPairCachingGhostObject* ghostObject;
    btKinematicCharacterController* bulletController;
};
};

