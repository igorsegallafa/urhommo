#pragma once

#include "UserManager.h"

#define USERMANAGER      (SERVERMANAGER->GetUserManager())

namespace Manager
{
class Server : public Impl
{
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    Server( Context* context );

    //! Deconstructor.
    ~Server();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    //! Getters.
    User* GetUserManager() const{ return userManager; }
private:
    SharedPtr<User> userManager;    //!< Pointer to user manager.
};
}
