#pragma once

class LoginHandler : public HandlerImpl
{
    URHO3D_OBJECT( LoginHandler, HandlerImpl );
public:
    //! Constructor.
    LoginHandler( Context* context );

    //! Deconstructor.
    ~LoginHandler();

    /**
     * Process Account Login for Login Server
     * @param account Account Name
     * @param password Password
     */
    void ProcessLogin( const String& account, const String& password );

    //! Handle Login Data Message.
    bool HandleLoginData( Connection* connection, MemoryBuffer& message );
};