#pragma once

#include "Screen.h"

#include "LoginScreen.h"
#include "CharacterScreen.h"
#include "TestScreen.h"
#include "WorldScreen.h"

#define ACTIVESCREEN			(SCREENMANAGER->GetActiveScreen())

#define SCREEN_TYPE             (SCREENMANAGER->GetActiveScreenType())

#define LOGINSCREEN             ((LoginScreen*)SCREENMANAGER->GetActiveScreen())
#define CHARACTERSCREEN         ((CharacterScreen*)SCREENMANAGER->GetActiveScreen())
#define WORLDSCREEN             ((WorldScreen*)SCREENMANAGER->GetActiveScreen())
#define TESTSCREEN              ((TestScreen*)SCREENMANAGER->GetActiveScreen())

class ScreenManager : public ManagerImpl
{
    URHO3D_OBJECT( ScreenManager, ManagerImpl );

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
    ScreenManager( Context* context );

    //! Deconstructor.
    ~ScreenManager();

    //! Initialize Screen Manager.
    bool Init();

    //! UnInitialize Screen Manager.
    void UnInit();

    //! Handlers.
    void HandleUpdate( StringHash eventType, VariantMap& eventData );

    //! Getters.
    Screen* GetActiveScreen() const{ return activeScreen_; }
    const ScreenType& GetActiveScreenType() const{ return activeScreenType_; }
    
    //! Setters.
    void ChangeScreen( const ScreenType& screen );
private:
    //! Scene Handlers.
    void HandleSetLevelQueue( StringHash eventType, VariantMap& eventData );
    void HandleResolutionChange( StringHash eventType, VariantMap& eventData );

    //! Build Window.
    void BuildWindow();
private:
    List<ScreenType> screenQueue_;
    SharedPtr<Screen> activeScreen_;
    ScreenType activeScreenType_;
    SharedPtr<Window> window_;

    float fadeTime_;
    FadeStep fadeStep_;
};