#pragma once

class User : public Core::User
{
    URHO3D_OBJECT( User, Core::User );
public:
    //! Constructor.
    User( Context* context );

    //! Deconstructor.
    ~User();

    //! Gettters and Setters.
    const String& GetCharacterName() const { return characterName; }
    void SetCharacterName( const String& characterName_ ) { characterName = characterName_; }
private:
    String characterName;   //!< Character Name.
};