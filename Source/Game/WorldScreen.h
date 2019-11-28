#pragma once

#include "Screen.h"

class WorldScreen : public Screen
{
    URHO3D_OBJECT( WorldScreen, Screen );
public:
    //! Constructor.
    WorldScreen( Context* context );

    //! Deconstructor.
    ~WorldScreen();

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