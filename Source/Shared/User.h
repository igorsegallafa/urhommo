#pragma once

namespace Core
{
class User : public Object
{
    URHO3D_OBJECT( User, Object );
public:
    //! Constructor.
    User( Context* context );

    //! Deconstructor.
    ~User();
public:
    int id_;    //!< ID.
    String accountName_; //!< Account Name.
    Connection* connection_; //!< Connection pointer.
};
}