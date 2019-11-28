#pragma once

namespace Handler
{
class Login : public Impl
{
    URHO3D_OBJECT( Login, Impl );
public:
    //! Constructor.
    Login( Context* context );

    //! Deconstructor.
    ~Login();

    /**
     * Process Account Login for Login Server
     * @param account Account Name
     * @param password Password
     */
    void ProcessLogin( const String& account, const String& password );

    //! Handle Login Data Message.
    bool HandleLoginData( Connection* connection, MemoryBuffer& message );
};
}
