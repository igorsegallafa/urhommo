#pragma once

#include <Urho3D/Network/Connection.h>

namespace Net
{

enum class ServerType
{
    Master,
    Login,
    Game,
    Proxy,
};

struct NetConnection
{
    int id;
    String name;
    String ip;
    int port;
    ServerType serverType;
    SharedPtr<Connection> connection;
};

const char* ServerTypeToString( const ServerType& serverType );

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

    //! Clear Connections.
    void Clear(){ connections.Clear(); }

    //! Load Server Configuration.
    bool Load( ServerType serverType );
private:
    Vector<NetConnection> connections;
};
}