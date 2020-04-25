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
    String characterName_;   //!< Character Name.
};