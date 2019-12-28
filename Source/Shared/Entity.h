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
public:
    Core::AnimationEntity* animationMgr;
protected:
    float height;
    float diameter;

    btPairCachingGhostObject* ghostObject;
    btKinematicCharacterController* bulletController;
};
};

