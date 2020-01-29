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

    //! Character Getter.
    Core::Character* GetCharacter() { return character; }

    //! Load Character.
    void LoadCharacter();

    //! Animation Handler.
    void ChangeAnimation( int animationID ){ animationToSet = animationID; }
    void ChangeAnimation( const Core::AnimationType& animationType );
private:
    void HandleUpdate( StringHash eventType, VariantMap& eventData );
	void HandlePostUpdate( StringHash eventType, VariantMap& eventData );
    void HandleMouseDown( StringHash eventType, VariantMap& eventData );
    void HandleNetworkUpdateSent( StringHash eventType, VariantMap& eventData );
private:
    int characterNodeID;	//!< Character Node ID.
    MapID mapIDToLoad;  //!< Map ID.
    WeakPtr<Core::Character> character; //!< Pointer for Character Component.
    Node* selectedNode; //!< Pointer for Selected Node.
    bool isWalking; //!< Character is Walking.
    int animationToSet;    //!< Character Animation.
};