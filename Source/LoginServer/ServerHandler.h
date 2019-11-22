#pragma once

#include "NetworkHandler.h"

#define NETWORKHANDLER      (SERVERHANDLER->GetNetworkHandler())

namespace Handler
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
    Handler::Network* GetNetworkHandler() const{ return networkHandler; }
private:
    SharedPtr<Handler::Network> networkHandler; //!< Network Handler Pointer
};
}
