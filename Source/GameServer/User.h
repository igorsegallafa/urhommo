#pragma once

class User : public Core::User
{
    URHO3D_OBJECT( User, Core::User );
public:
    //! Constructor.
    User( Context* context );

    //! Deconstructor.
    ~User();
public:
    String characterName_;          //!< Character Name.
    MapID mapID_;                   //!< Map ID.
    CharacterClass characterClass_; //!< Character Class.
    Core::Character* character_;    //!< Character Pointer.
};