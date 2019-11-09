#pragma once

#include "NetworkManager.h"

namespace Manager
{
class Server : public Object
{
    URHO3D_OBJECT( Server, Object );
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    Server( Context* context_ );

    //! Deconstructor.
    ~Server();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();
};
}
