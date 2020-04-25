#pragma once

#include "Screen.h"

class CharacterScreen : public Screen
{
    struct CharacterData
    {
        String name;
        Node* node;
        CharacterClass characterClass;
        unsigned level;
    };

    URHO3D_OBJECT( CharacterScreen, Screen );
public:
    //! Constructor.
    CharacterScreen( Context* context );

    //! Deconstructor.
    ~CharacterScreen();
protected:
    //! Initialize Scene.
    void Init() override;
private:
    //! Build Character List.
    void BuildCharacterList();

    //! Load Character.
    void Load( CharacterData& characterData, int armorId, int headId, int index );

    //! Create Scene.
    void CreateScene();

    //! Setup Viewport.
    void SetupViewport();

    //! Build Window.
    void BuildWindow();

    //! Screen Mode Handler.
    void HandleScreenMode( StringHash eventType, VariantMap& eventData );

    //! Select Character Button Handler.
    void HandleSelectCharacterButtonPressed( int index );

    //! Select Character Class Button Handler.
    void HandleSelectCharacterClassButtonPressed( int index );

    //! Create Character Button Handler.
    void HandleCreateCharacterButtonPressed( StringHash eventType, VariantMap& eventData );
    
    //! Continue Button Handler.
    void HandleProceedButtonPressed( StringHash eventType, VariantMap& eventData );

    //! Back Button Handler.
    void HandleBackButtonPressed( StringHash eventType, VariantMap& eventData );

    //! Get Character Class Selected.
    const CharacterClass& GetCharacterClassSelected();
private:
    SharedPtr<UIElement> window_;
    Vector<CharacterData> characters_;

    int characterClassIndex;
    int characterSelectedIndex;
};