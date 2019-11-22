#pragma once

#include "ScreenManager.h"

#define SCREENMANAGER      (GAMEMANAGER->GetScreenManager())

namespace Manager
{
class Game : public Impl
{
    URHO3D_OBJECT( Game, Impl );
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    Game( Context* context );

    //! Deconstructor.
    ~Game();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    //! Getters.
    Screen* GetScreenManager() const{ return screenManager; }
private:
    SharedPtr<Screen> screenManager;    //!< Pointer for Screen Manager.
};
}
