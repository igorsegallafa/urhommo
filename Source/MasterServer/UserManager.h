#pragma once

class UserManager : public ManagerImpl
{
    URHO3D_OBJECT( UserManager, ManagerImpl );
public:
    //! Constructor.
    UserManager( Context* context );

    //! Deconstructor.
    ~UserManager();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    //! Get User by Connection.
    User* GetUser( Connection* connection ){ return users_[connection]; }

    //! Add an user for the list.
    SharedPtr<User> AddUser( Connection* connection );

    //! Del user from the list.
    void DelUser( Connection* connection );
private:
    HashMap<Connection*, SharedPtr<User>> users_;
};