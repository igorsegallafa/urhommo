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

class Character : public Entity
{
    URHO3D_OBJECT( Character, Entity );
public:
    //! Default Constructor.
    Character( Context* context );

    //! Deconstructor.
    ~Character();
};

};