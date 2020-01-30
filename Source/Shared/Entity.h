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

    //! Fixed Update Handler.
    void FixedUpdate( float time );

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
    void HandlePostRenderUpdate( StringHash eventType, VariantMap& eventData );
public:
    Core::AnimationEntity* animationMgr;
protected:
    float height;
    float diameter;

    bool followingTarget;
    Vector3 targetDirection;
    Vector3 targetPos;
    PODVector<Vector3> targetPath;

    btPairCachingGhostObject* ghostObject;
    btKinematicCharacterController* bulletController;
};
};

