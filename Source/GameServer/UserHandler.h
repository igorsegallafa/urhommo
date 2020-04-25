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

    //! World Data Handler.
    bool HandleWorldData( Connection* connection, MemoryBuffer& message );

    //! Load User Handler.
    bool HandleLoadUser( Connection* connection, MemoryBuffer& message );
private:
    //! Load Character.
    bool LoadCharacter( User* user, const MapID& mapID, const Vector3& position );

    //! Handle User Connection.
    void HandleUserConnected( StringHash eventType, VariantMap& eventData );

    //! Handle User Disconnection.
    void HandleUserDisconnected( StringHash eventType, VariantMap& eventData );
};

