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

    void ProcessLogin( const String& account, const String& password );
};
}
