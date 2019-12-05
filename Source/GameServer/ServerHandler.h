#pragma once

#include "NetworkHandler.h"

#define NETWORKHANDLER      (SERVERHANDLER->Get<NetworkHandler>())

#define IMPL_HANDLER(name)  handlers[name::GetTypeStatic()] = new name( context ); 

class ServerHandler : public HandlerImpl
{
    URHO3D_OBJECT( ServerHandler, HandlerImpl );
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    ServerHandler( Context* context );

    //! Deconstructor.
    ~ServerHandler();

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
    HashMap<StringHash, HandlerImpl*> handlers;  //!< Pointer for Handlers.
};