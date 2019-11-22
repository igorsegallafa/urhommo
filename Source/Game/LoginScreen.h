#pragma once

#include "Screen.h"

namespace Core
{
class LoginScreen : public Screen
{
    URHO3D_OBJECT( LoginScreen, Screen );
public:
    //! Constructor.
    LoginScreen( Context* context );

    //! Deconstructor.
    ~LoginScreen();

    //! Initialize Scene.
    void Init() override;
private:
    //! Create Scene.
    void CreateScene();

    //! Setup Viewport.
    void SetupViewport();

    //! Build Window.
    void BuildWindow();
};
}