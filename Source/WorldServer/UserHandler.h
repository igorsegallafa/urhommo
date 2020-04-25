#pragma once

class UserHandler : public HandlerImpl
{
    URHO3D_OBJECT( UserHandler, HandlerImpl );
public:
    //! Constructor.
    UserHandler( Context* context );

    //! Deconstructor.
    ~UserHandler();

    //! Initialize.
    bool Init() override;

    //! Load User Handler.
    bool HandleLoadUser( Connection* connection, MemoryBuffer& message );

    //! Handle User Connection.
    void HandleUserConnected( StringHash eventType, VariantMap& eventData );

    //! Handle User Disconnection.
    void HandleUserDisconnected( StringHash eventType, VariantMap& eventData );
};

