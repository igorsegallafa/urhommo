#pragma once

#include "Screen.h"

namespace Core
{
class WorldScreen : public Screen
{
    URHO3D_OBJECT( WorldScreen, Screen );
public:
    //! Constructor.
    WorldScreen( Context* context );

    //! Deconstructor.
    ~WorldScreen();
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