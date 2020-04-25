#pragma once

namespace Core
{
class EntityNavigation
{
public:
    //! Constructor.
    EntityNavigation() :
        followingTarget_( false ),
        targetDirection_( Vector3::ZERO ),
        targetPosition_( Vector3::ZERO )
    {
    }

    //! Deconstructor.
    ~EntityNavigation() {}

    //! Handle Navigation Node.
    void HandleNavigationNode();

    //! Set Target Position.
    void SetTargetPosition( const Vector3& dest );
    void ResetTargetPosition();

    //! Is Following Target Getter.
    bool IsFollowingTarget() const { return followingTarget_; }

    //! Target Direction Getter and Setter.
    void SetTargetDirection( const Vector3& targetDirection ) { targetDirection_ = targetDirection; }
    const Vector3& GetTargetDirection() const { return targetDirection_; }
protected:
    bool followingTarget_;
    Vector3 targetDirection_;
    Vector3 targetPosition_;
    PODVector<Vector3> targetPath_;
};
};