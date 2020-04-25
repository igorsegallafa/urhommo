#pragma once

class UserHandler : public HandlerImpl
{
    URHO3D_OBJECT( UserHandler, HandlerImpl );
public:
    //! Constructor.
    UserHandler( Context* context );

    //! Deconstructor.
    ~UserHandler();

    /**
     * Select Character
     * @param characterName Name of character to select
     */
    void SelectCharacter( const String& characterName );

    /**
     * Create Character
     * @param characterName Name to create the character
     * @param characterClass Class of character
     */
    void CreateCharacter( const String& characterName, const CharacterClass& characterClass );
};