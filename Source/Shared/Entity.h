#pragma once

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
private:
    float height;
    float diameter;

    btPairCachingGhostObject* ghostObject;
    btKinematicCharacterController* bulletController;
};
};

