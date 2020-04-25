#pragma once

#include "UserManager.h"
#include "ConfigManager.h"
#include "MapManager.h"
#include "DatabaseManager.h"

#define USERMANAGER         (SERVERMANAGER->Get<UserManager>())
#define CONFIGMANAGER       (SERVERMANAGER->Get<ConfigManager>())
#define MAPMANAGER          (SERVERMANAGER->Get<MapManager>())
#define DATABASEMANAGER     (SERVERMANAGER->Get<DatabaseManager>())
#define EQUIPMENTMANAGER    (SERVERMANAGER->Get<EquipmentManager>())

#define IMPL_MANAGER(name)  managers_[name::GetTypeStatic()] = new name( context ); 

class ServerManager : public ManagerImpl
{
    URHO3D_OBJECT( ServerManager, ManagerImpl );
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    ServerManager( Context* context );

    //! Deconstructor.
    ~ServerManager();

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
