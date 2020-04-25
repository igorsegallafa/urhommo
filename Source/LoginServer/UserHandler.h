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

    //! Select Character Handler.
    bool HandleSelectCharacter( Connection* connection, MemoryBuffer& message );

    //! Create Character Handler.
    bool HandleCreateCharacter( Connection* connection, MemoryBuffer& message );
private:
    //! Handle User Validation.
    void HandleUserValidation( StringHash eventType, VariantMap& eventData );

    //! Handle User Disconnection.
    void HandleUserDisconnected( StringHash eventType, VariantMap& eventData );

    /**
     * Process World Data for Game
     * @param connection Connection to Send World Data
     * @param characterName Character Name
     * @param characterClass Character Class
     * @param characterLevel Character Level
     * @param mapID Map ID
     * @param position Spawn Position
     */
    void ProcessWorldData( Connection* connection, const String&, const CharacterClass&, int, const MapID&, const Vector3& );
};

