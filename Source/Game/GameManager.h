#pragma once

#include "ScreenManager.h"
#include "CameraManager.h"

#define SCREENMANAGER       (GAMEMANAGER->Get<ScreenManager>())
#define CAMERAMANAGER       (GAMEMANAGER->Get<CameraManager>())

#define IMPL_MANAGER(name)  managers[name::GetTypeStatic()] = new name( context ); 

class GameManager : public ManagerImpl
{
    URHO3D_OBJECT( GameManager, ManagerImpl );
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    GameManager( Context* context );

    //! Deconstructor.
    ~GameManager();

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
    HashMap<StringHash, ManagerImpl*> managers;  //!< Pointer for Managers.
};