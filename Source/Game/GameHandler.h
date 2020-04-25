#pragma once

#include "LoginHandler.h"
#include "NetworkHandler.h"
#include "UserHandler.h"
#include "CharacterHandler.h"
#include "ChatHandler.h"

#define LOGINHANDLER        (GAMEHANDLER->Get<LoginHandler>())
#define NETWORKHANDLER      (GAMEHANDLER->Get<NetworkHandler>())
#define USERHANDLER         (GAMEHANDLER->Get<UserHandler>())
#define CHARACTERHANDLER    (GAMEHANDLER->Get<CharacterHandler>())
#define CHATHANDLER         (GAMEHANDLER->Get<ChatHandler>())

#define IMPL_HANDLER(name)  handlers_[name::GetTypeStatic()] = new name( context ); 

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
        auto it = handlers_.Find( T::GetTypeStatic() );

        if( it != handlers_.End() )
            return static_cast<T*>(it->second_);

        return nullptr;
    }
private:
    HashMap<StringHash, HandlerImpl*> handlers_;  //!< Pointer for Handlers.
};