#pragma once

class MapManager : public ManagerImpl
{
    URHO3D_OBJECT( MapManager, ManagerImpl );
public:
    //! Constructor.
    MapManager( Context* context );

    //! Deconstructor.
    ~MapManager();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    //! Map Getter.
    Map* GetScene( const MapID& mapID );
private:
    void Load();
private:
    HashMap<int, SharedPtr<Map>> maps;
};