#pragma once

#include "ScreenManager.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "ConfigManager.h"

#define SCREENMANAGER       (GAMEMANAGER->Get<ScreenManager>())
#define CAMERAMANAGER       (GAMEMANAGER->Get<CameraManager>())
#define MAPMANAGER          (GAMEMANAGER->Get<MapManager>())
#define CONFIGMANAGER       (GAMEMANAGER->Get<ConfigManager>())
#define EQUIPMENTMANAGER    (GAMEMANAGER->Get<EquipmentManager>())

#define IMPL_MANAGER(name)  managers_[name::GetTypeStatic()] = new name( context ); 

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
        auto it = managers_.Find( T::GetTypeStatic() );

        if( it != managers_.End() )
            return static_cast<T*>(it->second_);

        return nullptr;
    }
private:
    HashMap<StringHash, ManagerImpl*> managers_;  //!< Pointer for Managers.
};