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

    //! Scene Getter.
    Scene* GetScene( const MapID& mapID );
private:
    void Load( const MapID& mapID, const String& objectFile );
private:
    HashMap<int, SharedPtr<Scene>> scenes;
};