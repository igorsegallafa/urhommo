#pragma once

#define CONNECTIONL     (NETWORKHANDLER->GetLoginServerConnection())
#define CONNECTIONG     (NETWORKHANDLER->GetGameServerConnection())
#define CONNECTIONM     (NETWORKHANDLER->GetMasterServerConnection())

#define MESSAGEHANDLER  (NETWORKHANDLER->GetMessageHandler())

#define HANDLE_MESSAGE(function,ptr)   std::bind( function, ptr, std::placeholders::_1, std::placeholders::_2 )

class NetworkHandler : public HandlerImpl
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

    //! Connect to Game Server.
    void ConnectGameServer( const String& ip, unsigned int port, VariantMap& identity );

    //! Close All connections.
    void CloseConnections();

    //! Close Login Server Connection.
    void CloseLoginServer();

    //! Close Master Server Connection.
    void CloseMasterServer();

    //! Close Game Server Connection.
    void CloseGameServer();

    //! Getters.
    Handler::Message* GetMessageHandler() const{ return messageHandler; }
    Connection* GetLoginServerConnection() const { return loginServerConnection; }
    Connection* GetGameServerConnection() const { return gameServerConnection; }
    Connection* GetMasterServerConnection() const { return masterServerConnection; }
private:
    Connection* Connect( const String& ip, unsigned int port, VariantMap& identity );

    //! Handle Server Connection.
    void HandleServerConnected( StringHash eventType, VariantMap& eventData );

    //! Handle Server Disconnected.
    void HandleServerDisconnected( StringHash eventType, VariantMap& eventData );
private:
    SharedPtr<Handler::Message> messageHandler; //!< Pointer for the Message Handler.

    Connection* loginServerConnection;  //!< Pointer for Login Server Connection.
    Connection* masterServerConnection; //!< Pointer for Master Server Connection.
    Connection* gameServerConnection;   //!< Pointer for Game Server Connection.
};