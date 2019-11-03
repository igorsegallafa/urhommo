#pragma once

namespace Net::Manager
{
class Server : public Object
{
    URHO3D_OBJECT( Server, Object );
public:
    //! Register Object as Subsystem.
    static void RegisterServerLibrary( Context* context_ );

    //! Constructor.
    Server( Context* context_ );

    //! Deconstructor.
    ~Server();
};
}