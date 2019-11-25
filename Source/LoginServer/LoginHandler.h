#pragma once

#include "../Shared/LoginDef.h"

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

    //! Initialize Object.
    bool Init() override;
private:
    void HandleClientIdentity( StringHash eventType, VariantMap& eventData );
};
}
