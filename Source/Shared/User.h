#pragma once

namespace Shared::Core
{
class User : public Object
{
    URHO3D_OBJECT( User, Object );
public:
    //! Constructor.
    User( Context* context );

    //! Deconstructor.
    ~User();
    
    //! Getters.
    const String& GetAccountName() const{ return accountName; }
    Connection* GetConnection() const{ return connection; }
private:
    String accountName; //!< Account Name.
    Connection* connection; //!< Connection pointer.
};
}