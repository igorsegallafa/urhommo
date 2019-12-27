#pragma once

class CharacterHandler : public HandlerImpl
{
    URHO3D_OBJECT( CharacterHandler, HandlerImpl );
public:
    //! Constructor.
    CharacterHandler( Context* context );

    //! Deconstructor.
    ~CharacterHandler();

    //! World Data Handler.
    bool HandleWorldData( Connection* connection, MemoryBuffer& message );

	//! Character Node ID Getter.
	int GetCharacterNodeID() const { return characterNodeID; }

    //! Load Character.
    void LoadCharacter();
private:
    void HandleUpdate( StringHash eventType, VariantMap& eventData );
	void HandlePostUpdate( StringHash eventType, VariantMap& eventData );
    void HandleMouseDown( StringHash eventType, VariantMap& eventData );
private:
    int characterNodeID;	//!< Character Node ID.
    MapID mapIDToLoad;  //!< Map ID.
    WeakPtr<Core::Character> character; //!< Pointer for Character Component.
    bool isWalking; //!< Character is Walking.
};