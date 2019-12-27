#pragma once

#include "Entity.h"

namespace Core
{
enum class CharacterClass
{
    Undefined,

    Fighter,
    Mechanician,
    Archer,
    Pikeman,

    Knight,
    Atalanta,
    Magician,
    Priestess,
};

enum CharacterControl
{
    CHARACTERCONTROL_Forward = 1 << 1,
};

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
    Connection* connection; //!< Connection Pointer.
};

};