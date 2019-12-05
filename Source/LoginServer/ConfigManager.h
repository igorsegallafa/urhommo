#pragma once

class ConfigManager : public ManagerImpl
{
    URHO3D_OBJECT(ConfigManager, ManagerImpl);
public:
    //! Constructor.
    ConfigManager(Context* context);

    //! Deconstructor.
    ~ConfigManager();

    //! Initialize Object.
    bool Init();

    //! Load Configuration.
    bool Load( const Net::ServerType& serverType, int id = 0 );

    //! Get Current Net Server Connection.
    Net::NetConnection* GetNetConnection() { return connection; }

    //! Get Net Server Configuration.
    Net::NetConnection* GetNetConfig( const Net::ServerType& serverType, int id = 0 );

    //! Get All Net Server Connections.
    const Vector<Net::NetConnection*> GetNetConnections() const { return netConnections; }
private:
    Net::NetConnection* connection;
    Vector<Net::NetConnection*> netConnections;
};