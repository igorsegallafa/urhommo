#include "PrecompiledHeader.h"
#include "MapManager.h"

MapManager::MapManager( Context* context ) :
    ManagerImpl( context ),
    maps{}
{
}

MapManager::~MapManager()
{
    maps.Clear();
}

bool MapManager::Init()
{
    BaseMap* map = nullptr;

    //TODO: Load Maps from SQL

    //Ricarten Town
    map = maps[(MAP_ID)MapID::Ricarten] = new BaseMap( context_ );
    map->name = "Ricarten Town";
    map->objectFile = "Objects/s_v2/s_v2.xml";

    //Garden of Freedom
    map = maps[(MAP_ID)MapID::GardenOfFreedom] = new BaseMap( context_ );
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

BaseMap* MapManager::GetMap( const MapID& mapID )
{
    auto it = maps.Find( (MAP_ID)mapID );

    if( it != maps.End() )
        return it->second_;

    return nullptr;
}

void MapManager::Load()
{
    for( auto& map : maps )
    {
        //Create Scene
		map.second_->scene = new Scene( context_ );
		map.second_->scene->CreateComponent<Octree>( LOCAL );
		map.second_->scene->CreateComponent<PhysicsWorld>( LOCAL );

        //Instantiate Map Object
		map.second_->scene->InstantiateXML( RESOURCECACHE->GetResource<XMLFile>( map.second_->objectFile )->GetRoot(), map.second_->centerPosition, Quaternion::IDENTITY, LOCAL );
    }
}
