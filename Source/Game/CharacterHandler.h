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
	int GetCharacterNodeID() const { return characterNodeID_; }

    //! Character Getter.
    Core::Character* GetCharacter() { return character_; }

    //! Load Character.
    void LoadCharacter();

    //! UnLoad.
    void UnLoad();

    //! Selected Node Getter.
    Node* GetSelectedNode() const{ return selectedNode_; }

    //! Hovered Node Getter.
    Node* GetHoveredNode() const{ return hoveredNode_; }

    //! Animation Handler.
    void ChangeAnimation( int animationID, bool exclusive = false ){ animationToSet_ = Pair(animationID, exclusive); }
    void ChangeAnimation( const Core::AnimationType& animationType, bool exclusive = false );
private:
    void HandleUpdate( StringHash eventType, VariantMap& eventData );
	void HandlePostUpdate( StringHash eventType, VariantMap& eventData );
    void HandleMouseDown( StringHash eventType, VariantMap& eventData );
    void HandleNetworkUpdateSent( StringHash eventType, VariantMap& eventData );
private:
    int characterNodeID_;	//!< Character Node ID.
    MapID mapIDToLoad_;  //!< Map ID.
    WeakPtr<Core::Character> character_; //!< Pointer for Character Component.
    Node* selectedNode_; //!< Pointer for Selected Node.
    Node* hoveredNode_;  //!< Pointer for Hovered Node.
    bool isWalking_; //!< Character is Walking.
    Pair<int,bool> animationToSet_;    //!< Character Animation.
};