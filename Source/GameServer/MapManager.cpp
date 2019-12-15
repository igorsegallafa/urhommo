#include "PrecompiledHeader.h"
#include "MapManager.h"

MapManager::MapManager( Context* context ) :
    ManagerImpl( context ),
    maps{}
{
}

MapManager::~MapManager()
{
}

bool MapManager::Init()
{
    Map* map = nullptr;

    //Ricarten Town
    map = maps[(MAP_ID)MapID::Ricarten] = new Map( context_ );
    map->name = "Ricarten Town";
    map->objectFile = "Objects/s_v2/s_v2.xml";

    //Garden of Freedom
    map = maps[(MAP_ID)MapID::GardenOfFreedom] = new Map( context_ );
    map->name = "Garden of Freedom";
    map->objectFile = "Objects/s_f/s_f_01.xml";

    //Load Maps
    Load();

    return true;
}

void MapManager::UnInit()
{
    maps.Clear();
}

Map* MapManager::GetScene( const MapID& mapID )
{
    auto it = maps.Find( (int)mapID );

    if( it != maps.End() )
        return it->second_;

    return nullptr;
}

void MapManager::Load()
{
    for( auto& map : maps )
    {
        //Create Scene
        SharedPtr<Scene> scene( new Scene( context_ ) );
        scene->CreateComponent<Octree>( LOCAL );
        scene->CreateComponent<PhysicsWorld>( LOCAL );

        //Instantiate Map Object
        scene->InstantiateXML( RESOURCECACHE->GetResource<XMLFile>( map.second_->objectFile )->GetRoot(), map.second_->centerPosition, Quaternion::IDENTITY, LOCAL );
    
        //Set Scene
        map.second_->scene = scene;
    }
}
