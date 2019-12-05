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
    
    //! Getters and Setters.
    void SetAccountName( const String& accountName_ ) { accountName = accountName_; }
    const String& GetAccountName() const{ return accountName; }

    void SetConnection( Connection* connection_ ){ connection = connection_; }
    Connection* GetConnection() const{ return connection; }
private:
    String accountName; //!< Account Name.
    Connection* connection; //!< Connection pointer.
};
}