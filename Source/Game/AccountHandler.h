#pragma once

class AccountHandler : public HandlerImpl
{
    URHO3D_OBJECT( AccountHandler, HandlerImpl );
public:
    //! Constructor.
    AccountHandler( Context* context );

    //! Deconstructor.
    ~AccountHandler();

    /**
     * Create Character
     * @param characterName Name to create the character
     * @param characterClass Class of character
     */
    void CreateCharacter( const String& characterName, const Core::CharacterClass& characterClass );
};