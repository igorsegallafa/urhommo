#pragma once

#include "LoginHandler.h"
#include "NetworkHandler.h"
#include "AccountHandler.h"

#define LOGINHANDLER        (GAMEHANDLER->Get<LoginHandler>())
#define NETWORKHANDLER      (GAMEHANDLER->Get<NetworkHandler>())
#define ACCOUNTHANDLER      (GAMEHANDLER->Get<AccountHandler>())

#define IMPL_HANDLER(name)  handlers[name::GetTypeStatic()] = new name( context ); 

class GameHandler : public HandlerImpl
{
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    GameHandler( Context* context );

    //! Deconstructor.
    ~GameHandler();

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