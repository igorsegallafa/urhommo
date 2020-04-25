#pragma once

#define NETSERVER       (NETWORKHANDLER->GetNetServer())
#define MESSAGE_HANDLER(handler_class, function)    std::bind( &handler_class::function, SERVERHANDLER->Get<handler_class>(), std::placeholders::_1, std::placeholders::_2 )

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

    //! Getters.
    Net::Server* GetNetServer() const{ return netServer_; }
private:
    //! Validate Message.
    bool CanProcessMessage( int messageID, Connection* connection );
private:
    SharedPtr<Net::Server> netServer_;   //!< Pointer for the Net Server.
};