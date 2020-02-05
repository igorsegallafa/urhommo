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

    //! UnLoad.
    void UnLoad();

    //! Selected Node Getter.
    Node* GetSelectedNode() const{ return selectedNode; }

    //! Hovered Node Getter.
    Node* GetHoveredNode() const{ return hoveredNode; }

    //! Animation Handler.
    void ChangeAnimation( int animationID, bool exclusive = false ){ animationToSet = Pair(animationID, exclusive); }
    void ChangeAnimation( const Core::AnimationType& animationType, bool exclusive = false );
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
    Node* hoveredNode;  //!< Pointer for Hovered Node.
    bool isWalking; //!< Character is Walking.
    Pair<int,bool> animationToSet;    //!< Character Animation.
};