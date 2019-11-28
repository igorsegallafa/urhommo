#pragma once

#include "Screen.h"

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

    //! Set Game Server List.
    void SetGameServerList( const Vector<String>& gameServerList );
private:
    //! Create Scene.
    void CreateScene();

    //! Setup Viewport.
    void SetupViewport();

    //! Build Window.
    void BuildWindow();

    //! Game Server Selected Handler.
    void HandleGameServerPressed( int serverIndex );

    //! Login Button Handler.
    void HandleLoginButtonPressed( StringHash eventType, VariantMap& eventData );
private:
    SharedPtr<UIElement> loginWindow;
    SharedPtr<UIElement> gameServerWindow;
};