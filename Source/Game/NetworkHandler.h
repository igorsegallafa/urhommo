#pragma once

#define MESSAGEHANDLER  (NETWORKHANDLER->GetMessageHandler())

namespace Handler
{
class Network : public Impl
{
    URHO3D_OBJECT( Network, Impl );
public:
    //! Constructor.
    Network( Context* context );

    //! Deconstructor.
    ~Network();

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
private:
    Connection* Connect( const String& ip, unsigned int port, VariantMap& identity );

    //! Handle Client Connection Identity.
    void HandleClientIdentity( StringHash eventType, VariantMap& eventData );

    //! Handle New Client Connection.
    void HandleClientConnected( StringHash eventType, VariantMap& eventData );

    //! Handle Client Disconnection.
    void HandleClientDisconnected( StringHash eventType, VariantMap& eventData );

    //! Handle Network Message.
    void HandleMessage( StringHash eventType, VariantMap& eventData );
private:
    SharedPtr<Handler::Message> messageHandler; //!< Pointer for the Message Handler.

    Connection* loginServerConnection;  //!< Pointer for Login Server Connection.
    Connection* masterServerConnection; //!< Pointer for Master Server Connection.
    Connection* gameServerConnection;   //!< Pointer for Game Server Connection.
};
}
