#pragma once

class AccountHandler : public HandlerImpl
{
    URHO3D_OBJECT( AccountHandler, HandlerImpl );
public:
    //! Constructor.
    AccountHandler( Context* context );

    //! Deconstructor.
    ~AccountHandler();

    //! Create Character Handler
    bool HandleCreateCharacter( Connection* connection, MemoryBuffer& message );

    /**
     * Process World Data for Game
     * @param connection Connection to Send World Data
     * @param characterName Character Name
     * @param characterClass Character Class
     * @param characterLevel Character Level
     * @param position Spawn Position
     */
    void ProcessWorldData( Connection* connection, const String&, const Core::CharacterClass&, int, const Vector3& );
};