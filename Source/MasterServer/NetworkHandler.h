#pragma once

#define NETSERVER       (NETWORKHANDLER->GetNetServer())
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

    //! Getters.
    Net::Server* GetNetServer() const{ return netServer; }
    Handler::Message* GetMessageHandler() const{ return messageHandler; }
private:
    //! Validate Message.
    bool CanProcessMessage( int messageID, Connection* connection );

    //! Handle Client Connection Identity.
    void HandleClientIdentity( StringHash eventType, VariantMap& eventData );

    //! Handle New Client Connection.
    void HandleClientConnected( StringHash eventType, VariantMap& eventData );

    //! Handle Client Disconnection.
    void HandleClientDisconnected( StringHash eventType, VariantMap& eventData );

    //! Handle Network Message.
    void HandleMessage( StringHash eventType, VariantMap& eventData );
private:
    SharedPtr<Net::Server> netServer;   //!< Pointer for the Net Server.
    SharedPtr<Handler::Message> messageHandler; //!< Pointer for the Message Handler.
};