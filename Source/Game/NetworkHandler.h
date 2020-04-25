#pragma once

#define CONNECTIONL     (NETWORKHANDLER->GetLoginServerConnection())
#define CONNECTIONG     (NETWORKHANDLER->GetGameServerConnection())
#define CONNECTIONM     (NETWORKHANDLER->GetMasterServerConnection())
#define CONNECTIONW     (NETWORKHANDLER->GetWorldServerConnection())

#define MESSAGE_HANDLER(handler_class, function)    std::bind( &handler_class::function, GAMEHANDLER->Get<handler_class>(), std::placeholders::_1, std::placeholders::_2 )

class NetworkHandler : public HandlerImpl, private Handler::Message
{
    URHO3D_OBJECT( NetworkHandler, HandlerImpl );
public:
    //! Constructor.
    NetworkHandler( Context* context );

    //! Deconstructor.
    ~NetworkHandler();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    //! Connect to Login Server.
    void ConnectLoginServer( const String& ip, unsigned int port, VariantMap& identity );

    //! Connect to Master Server.
    void ConnectMasterServer( const String& ip, unsigned int port, VariantMap& identity );

    //! Connect to World Server.
    void ConnectWorldServer( const String& ip, unsigned int port, VariantMap& identity );

    //! Connect to Game Server.
    void ConnectGameServer( const String& ip, unsigned int port, VariantMap& identity );

    //! Close All connections.
    void CloseConnections();

    //! Close Login Server Connection.
    void CloseLoginServer();

    //! Close Master Server Connection.
    void CloseMasterServer();

    //! Close World Server Connection.
    void CloseWorldServer();

    //! Close Game Server Connection.
    void CloseGameServer();

    //! Getters.
    Connection* GetLoginServerConnection() const { return loginServerConnection_; }
    Connection* GetWorldServerConnection() const { return worldServerConnection_; }
    Connection* GetGameServerConnection() const { return gameServerConnection_; }
    Connection* GetMasterServerConnection() const { return masterServerConnection_; }
private:
    Connection* Connect( const String& ip, unsigned int port, VariantMap& identity );

    //! Handle Server Connection.
    void HandleServerConnected( StringHash eventType, VariantMap& eventData );

    //! Handle Server Disconnected.
    void HandleServerDisconnected( StringHash eventType, VariantMap& eventData );
private:
    Connection* loginServerConnection_;  //!< Pointer for Login Server Connection.
    Connection* masterServerConnection_; //!< Pointer for Master Server Connection.
    Connection* worldServerConnection_;  //!< Pointer for World Server Connection.
    Connection* gameServerConnection_;   //!< Pointer for Game Server Connection.
};