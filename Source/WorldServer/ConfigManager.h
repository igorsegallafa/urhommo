#pragma once

//! Config Parameters.
static const String CP_SERVERID = "ServerID";

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
    Net::NetConnection* GetNetConnection() { return connection_; }

    //! Get Net Server Configuration.
    Net::NetConnection* GetNetConfig( const Net::ServerType& serverType, int id = 0 );

    //! Get All Net Server Connections.
    const Vector<Net::NetConnection*> GetNetConnections() const { return netConnections_; }
private:
    void ParseParameters();
private:
    VariantMap parameters_;
    Net::NetConnection* connection_;
    Vector<Net::NetConnection*> netConnections_;
};