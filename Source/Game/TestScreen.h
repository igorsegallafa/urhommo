#pragma once

#include "Screen.h"

class TestScreen : public Screen
{
    URHO3D_OBJECT( TestScreen, Screen );
public:
    //! Constructor.
    TestScreen( Context* context );

    //! Deconstructor.
    ~TestScreen();

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