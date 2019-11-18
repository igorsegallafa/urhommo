#pragma once

namespace Manager
{
class User : public Impl
{
public:
    //! Constructor.
    User( Context* context_ );

    //! Deconstructor.
    ~User();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    //! Get User by Connection.
    Core::User* GetUser( Connection* connection ){ return users[connection]; }

    //! Add an user for the list.
    SharedPtr<Core::User> AddUser( Connection* connection );

    //! Del user from the list.
    void DelUser( Connection* connection );
private:
    HashMap<Connection*, SharedPtr<Core::User>> users;
};
}