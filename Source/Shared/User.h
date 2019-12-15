#pragma once

namespace Core
{
enum class LoginStatus
{
    Undefined,

    Successful,
    Banned,
    WrongAccount,
    WrongPassword,
};

class User : public Object
{
    URHO3D_OBJECT( User, Object );
public:
    //! Constructor.
    User( Context* context );

    //! Deconstructor.
    ~User();
public:
    String accountName; //!< Account Name.
    Connection* connection; //!< Connection pointer.
};
}