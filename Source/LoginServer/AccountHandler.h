#pragma once

class AccountHandler : public HandlerImpl
{
    URHO3D_OBJECT( AccountHandler, HandlerImpl );
public:
    //! Constructor.
    AccountHandler( Context* context );

    //! Deconstructor.
    ~AccountHandler();
};