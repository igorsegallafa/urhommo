#include "PrecompiledHeader.h"
#include "MapManager.h"

MapManager::MapManager( Context* context ) :
    ManagerImpl( context ),
    maps_{},
    scene_( nullptr )
{
}

MapManager::~MapManager()
{
    maps_.Clear();
}

bool MapManager::Init()
{
    BaseMap* map = nullptr;

    //TODO: Load Maps from SQL

    //Ricarten Town
    map = maps_[(MAP_ID)MapID::Ricarten] = new BaseMap( context_ );
    map->name_ = "Ricarten Town";
    map->objectFile_ = "Objects/s_v2/s_v2.xml";

    //Garden of Freedom
    map = maps_[(MAP_ID)MapID::GardenOfFreedom] = new BaseMap( context_ );
    map->name_ = "Garden of Freedom";
    map->objectFile_ = "Objects/s_f/s_f_01.xml";

    //Load Maps
    Load();

    return true;
}

void MapManager::UnInit()
{
    maps_.Clear();
}

BaseMap* MapManager::GetMap( const MapID& mapID )
{
    auto it = maps_.Find( (MAP_ID)mapID );

    if( it != maps_.End() )
        return it->second_;

    return nullptr;
}

void MapManager::Load()
{
    scene_ = new Scene( context_ );
    scene_->CreateComponent<Octree>( LOCAL );
    scene_->CreateComponent<PhysicsWorld>( LOCAL );

    for( auto& map : maps_ )
        scene_->InstantiateXML( RESOURCECACHE->GetResource<XMLFile>( map.second_->objectFile_ )->GetRoot(), map.second_->centerPosition_, Quaternion::IDENTITY, LOCAL );
}
