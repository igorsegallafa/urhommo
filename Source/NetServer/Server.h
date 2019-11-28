#pragma once

#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>

namespace Net
{
/**
 * Custom Parameters for Client Identity Event 
 */
URHO3D_PARAM( P_SERVERTYPE, ServerType );
URHO3D_PARAM( P_SERVERID, ServerID );

enum class ServerType
{
    Undefined,
    Master,
    Login,
    Game,
};

struct NetConnection
{
    int id; //!< Connection ID
    String name;    //!< Server Name
    String ip;  //!< Server IP
    int port;   //!< Server Port
    ServerType serverType;  //!< Server Type
    Connection* connection;   //!< Connection Pointer
};

const char* ServerTypeToString( const ServerType& serverType );

class Server : public Object
{
    URHO3D_OBJECT( Server, Object );
public:
    //! Constructor.
    Server( Context* context );

    //! Deconstructor.
    ~Server();

    //! Initialize Net Server.
    bool Init();

    //! UnInitialize.
    void UnInit();

    //! Start Server.
    bool Start( ServerType serverType, int index = 0 );

    //! Load Server Configuration.
    bool Load( ServerType serverType );

    //! Connect with all Servers loaded.
    bool ConnectAll();

    //! Get Connections.
    const Urho3D::Vector<NetConnection>& GetConnections() const{ return connections; }

    //! Get a specific connection by Server Type.
    NetConnection* GetConnection( ServerType serverType, int index = 0 ) const;

    //! Handle Client Connection Identity.
    void HandleClientIdentity( StringHash eventType, VariantMap& eventData );

    //! Handle Server Connection Status.
    void HandleConnectionStatus( StringHash eventType, VariantMap& eventData );
private:
    int id;   //!< Current Server ID.
    ServerType type;  //!< Current Server Type.

    Connection* serverConnection;  //!< Server Connection.
    Urho3D::Vector<NetConnection> connections;  //!< Net Connections Opened.
};
}