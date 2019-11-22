#pragma once

#include "Screen.h"

namespace Core
{
class TestScreen : public Screen
{
    URHO3D_OBJECT( TestScreen, Screen );
public:
    //! Constructor.
    TestScreen( Context* context );

    //! Deconstructor.
    ~TestScreen();
protected:
    //! Initialize Scene.
    virtual void Init();
private:
    //! Create Scene.
    void CreateScene();

    //! Setup Viewport.
    void SetupViewport();

    //! Build Window.
    void BuildWindow();
};
}