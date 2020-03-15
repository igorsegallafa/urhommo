#pragma once

namespace Core
{
class EntityNavigation
{
public:
    //! Constructor.
    EntityNavigation() :
        followingTarget( false ),
        targetDirection( Vector3::ZERO ),
        targetPos( Vector3::ZERO )
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
    bool IsFollowingTarget() const { return followingTarget; }

    //! Target Direction Getter and Setter.
    void SetTargetDirection( const Vector3& dir ) { targetDirection = dir; }
    const Vector3& GetTargetDirection() const { return targetDirection; }
protected:
    bool followingTarget;
    Vector3 targetDirection;
    Vector3 targetPos;
    PODVector<Vector3> targetPath;
};
};