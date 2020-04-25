#pragma once

#include "../Shared/LoginDef.h"

struct CharacterInfo
{
    String name;
    unsigned level;
    int characterClass;
    int armorId;
    int headId;
};

class LoginHandler : public HandlerImpl
{
    URHO3D_OBJECT( LoginHandler, HandlerImpl );
public:
    //! Constructor.
    LoginHandler( Context* context );

    //! Deconstructor.
    ~LoginHandler();

    //! User Validation Handler.
    const LoginStatusFlags HandleUserValidation( const String& account, const String& password, int& outUserID );

    //! User Login Handler.
    const LoginStatusFlags HandleUserLogin( User* user );

    //! Process Login Response.
    void ProcessLoginResponse( const LoginStatusFlags& loginStatus, User* user );
private:
    inline bool IsAccountLogged( User* user );

    //! Get Characters from Account.
    Vector<CharacterInfo>& GetCharactersFromAccount( int userId );
};