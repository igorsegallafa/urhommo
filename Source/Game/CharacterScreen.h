#pragma once

#include "Screen.h"

class CharacterScreen : public Screen
{
    URHO3D_OBJECT( CharacterScreen, Screen );
public:
    //! Constructor.
    CharacterScreen( Context* context );

    //! Deconstructor.
    ~CharacterScreen();
protected:
    //! Initialize Scene.
    void Init() override;
private:
    //! Create Scene.
    void CreateScene();

    //! Setup Viewport.
    void SetupViewport();

    //! Build Window.
    void BuildWindow();
private:
    SharedPtr<UIElement> window;
};