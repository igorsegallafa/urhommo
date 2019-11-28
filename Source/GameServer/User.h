#pragma once

namespace Core
{
class User : public Shared::Core::User
{
    URHO3D_OBJECT( User, Shared::Core::User );
public:
    //! Constructor.
    User( Context* context );

    //! Deconstructor.
    ~User();
};
}