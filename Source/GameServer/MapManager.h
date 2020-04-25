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
    BaseMap* GetMap( const MapID& mapID );

    //! Scene Getter.
    Scene* GetScene() const{ return scene_; }
private:
    void Load();
private:
    HashMap<int, SharedPtr<BaseMap>> maps_;
    SharedPtr<Scene> scene_;
};