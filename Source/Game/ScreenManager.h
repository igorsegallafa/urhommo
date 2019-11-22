#pragma once

#include "Screen.h"

#include "LoginScreen.h"
#include "CharacterScreen.h"
#include "TestScreen.h"
#include "WorldScreen.h"

#define ACTIVESCREEN			(SCREENMANAGER->GetActiveScreen())

namespace Manager
{
class Screen : public Impl
{
    URHO3D_OBJECT( Screen, Impl );

    enum class FadeStep
    {
        Prepare,
        Out,
        ReleaseScreen,
        CreateScreen,
        In,
        Finished,
    };

public:
    //! Constructor.
    Screen( Context* context );

    //! Deconstructor.
    ~Screen();

    //! Initialize Screen Manager.
    bool Init();

    //! UnInitialize Screen Manager.
    void UnInit();

    //! Handlers.
    void HandleUpdate( StringHash eventType, VariantMap& eventData );

    //! Getters.
    Core::Screen* GetActiveScreen() const{ return activeScreen; }
    const Core::ScreenType& GetActiveScreenType() const{ return activeScreenType; }
    
    //! Setters.
    void SetActiveScreen( const Core::ScreenType& screen );
private:
    //! Scene Handlers.
    void HandleSetLevelQueue( StringHash eventType, VariantMap& eventData );
    void HandleResolutionChange( StringHash eventType, VariantMap& eventData );

    //! Build Window.
    void BuildWindow();
private:
    List<Core::ScreenType> screenQueue;
    SharedPtr<Core::Screen> activeScreen;
    Core::ScreenType activeScreenType;
    SharedPtr<Window> window;

    float fadeTime;
    FadeStep fadeStep;
};
}