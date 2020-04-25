#pragma once

#include "Entity.h"
#include "CharacterEquipment.h"

namespace Core
{
class Character : public Entity
{
    URHO3D_OBJECT( Character, Entity );
public:
    //! Default Constructor.
    Character( Context* context );

    //! Deconstructor.
    ~Character();

    //! Fixed Update Handler.
    void FixedUpdate( float time ) override;
public:
    Connection* connection_; //!< Connection Pointer.
    CharacterEquipment* equipmentMgr_;    //!< Character Equipment Manager.
};

};