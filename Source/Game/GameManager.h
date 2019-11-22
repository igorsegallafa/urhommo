#pragma once

#include "ScreenManager.h"
#include "CameraManager.h"

#define SCREENMANAGER       (GAMEMANAGER->Get<Manager::Screen>())
#define CAMERAMANAGER       (GAMEMANAGER->Get<Manager::Camera>())

#define IMPL_MANAGER(name)  managers[name::GetTypeStatic()] = new name( context ); 

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

    //! Manager Getter.
    template<class T>
    inline T* Get()
    {
        auto it = managers.Find( T::GetTypeStatic() );

        if( it != managers.End() )
            return static_cast<T*>(it->second_);

        return nullptr;
    }
private:
    HashMap<StringHash, Impl*> managers;  //!< Pointer for Managers.
};
}
