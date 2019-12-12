#include "PrecompiledHeader.h"
#include "MapManager.h"

MapManager::MapManager( Context* context ) :
    ManagerImpl( context ),
    scenes{}
{
}

MapManager::~MapManager()
{
}

bool MapManager::Init()
{
    Load( MapID::Ricarten, "Objects/s_v2/s_v2.xml" );
    Load( MapID::GardenOfFreedom, "Objects/s_f/s_f_01.xml" );

    return true;
}

void MapManager::UnInit()
{
    scenes.Clear();
}

Scene* MapManager::GetScene( const MapID& mapID )
{
    auto it = scenes.Find( (int)mapID );

    if( it != scenes.End() )
        return it->second_;

    return nullptr;
}

void MapManager::Load( const MapID& mapID, const String& objectFile )
{
    auto it = scenes.Find( (int)mapID );

    if( it != scenes.End() )
    {
        URHO3D_LOGERRORF( "Already exists some scene with this MapID (%d)", (int)mapID );
        return;
    }

    //Create Scene
    SharedPtr<Scene> scene( new Scene( context_ ) );
    scene->CreateComponent<Octree>( LOCAL );
    scene->CreateComponent<PhysicsWorld>( LOCAL );

    //Instantiate Map Object
    scene->InstantiateXML( RESOURCECACHE->GetResource<XMLFile>( objectFile )->GetRoot(), Vector3::ZERO, Quaternion::IDENTITY, LOCAL );
}
