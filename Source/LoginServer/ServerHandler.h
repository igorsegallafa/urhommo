#pragma once

#include "NetworkHandler.h"
#include "LoginHandler.h"

#define NETWORKHANDLER      (SERVERHANDLER->Get<Handler::Network>())
#define LOGINHANDLER        (SERVERHANDLER->Get<Handler::Login>())

#define IMPL_HANDLER(name)  handlers[name::GetTypeStatic()] = new name( context ); 

namespace Handler
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

    //! Handler Getter.
    template<class T>
    inline T* Get()
    {
        auto it = handlers.Find( T::GetTypeStatic() );

        if( it != handlers.End() )
            return static_cast<T*>(it->second_);

        return nullptr;
    }
private:
    HashMap<StringHash, Impl*> handlers;  //!< Pointer for Handlers.
};
}
