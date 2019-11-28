#pragma once

#include "UserManager.h"

#define USERMANAGER      (SERVERMANAGER->Get<UserManager>())

#define IMPL_MANAGER(name)  managers[name::GetTypeStatic()] = new name( context ); 

class ServerManager : public ManagerImpl
{
    URHO3D_OBJECT( ServerManager, ManagerImpl );
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    ServerManager( Context* context );

    //! Deconstructor.
    ~ServerManager();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    //! Manager Getter.
    template<class T>
    inline T* Get()
    {
        auto it = managers.Find( T::GetTypeStatic() );

        if( it != managers.End() )
            return static_cast<T*>(it->second_);

        return nullptr;
    }
private:
    HashMap<StringHash, ManagerImpl*> managers;  //!< Pointer for Managers.
};
