#pragma once

#include "Animation.h"

#include "EntityCollision.h"
#include "EntityNavigation.h"

namespace Core
{
class Entity : public LogicComponent, public EntityCollision, public EntityNavigation
{
    friend class EntityCollision;
    friend class EntityNavigation;

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

    //! Fixed Post Update Handler.
    void FixedPostUpdate( float time );
public:
    Core::AnimationEntity* animationMgr_;
};
};

