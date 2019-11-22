#pragma once

#include "UserManager.h"

#define USERMANAGER      (SERVERMANAGER->Get<Manager::User>())

#define IMPL_MANAGER(name)  managers[name::GetTypeStatic()] = new name( context ); 

namespace Manager
{
class Server : public Impl
{
    URHO3D_OBJECT( Server, Impl );
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
    HashMap<StringHash, Impl*> managers;  //!< Pointer for Managers.
};
}
