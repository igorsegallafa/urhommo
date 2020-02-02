#include "PrecompiledHeader.h"
#include "MapManager.h"

MapManager::MapManager( Context* context ) :
    ManagerImpl( context ),
    maps{},
    curMap( nullptr, MapID::Undefined ),
    nextMap( nullptr, MapID::Undefined )
{
}

MapManager::~MapManager()
{
    maps.Clear();
}

bool MapManager::Init()
{
    BaseMap* map = nullptr;
    MapID mapID = MapID::Undefined;

    //Ricarten Town
    mapID = MapID::Ricarten;
    map = maps[(MAP_ID)mapID] = new BaseMap( context_ );
    map->mapID = mapID;
    map->name = "Ricarten Town";
    map->objectFile = "Objects/s_v2/s_v2.xml";
    map->centerPosition = Vector3( 0.f, 0.f, 0.f );

    //Garden of Freedom
    mapID = MapID::GardenOfFreedom;
    map = maps[(MAP_ID)mapID] = new BaseMap( context_ );
    map->mapID = mapID;
    map->name = "Garden of Freedom";
    map->objectFile = "Objects/s_f/s_f_01.xml";
    map->centerPosition = Vector3( 86.313f, 11.5915f, 227.526f );

    //Bamboo Forest
    mapID = MapID::BambooForest;
    map = maps[(MAP_ID)mapID] = new BaseMap( context_ );
    map->mapID = mapID;
    map->name = "Bamboo Forest";
    map->objectFile = "Objects/s_f/s_f_02.xml";
    map->centerPosition = Vector3( -181.485f, 11.938f, 214.416f );

    return true;
}

void MapManager::UnInit()
{
    maps.Clear();
}

bool MapManager::Load( const MapID& mapID )
{
    if( SCREEN_TYPE == ScreenType::World )
    {
        auto it = maps.Find( (MAP_ID)mapID );

        if( it != maps.End() )
        {
            auto scene = WORLDSCREEN->GetScene();
            auto mapXML = RESOURCECACHE->GetResource<XMLFile>( it->second_->objectFile );
            auto tmpNode = scene->CreateTemporaryChild();

            SceneResolver resolver;

            //Load Map
            if( tmpNode->LoadXML( mapXML->GetRoot(), resolver, false, false, LOCAL ) )
            {
                curMap.first_ = scene->InstantiateXML( mapXML->GetRoot(), it->second_->centerPosition, Quaternion::IDENTITY, LOCAL );
                curMap.second_ = it->second_->mapID;
                NodeRegisterLoadTriggers( curMap.first_ );

                return true;
            }
        }
    }

    return false;
}

BaseMap* MapManager::GetMap( const MapID& mapID )
{
    auto it = maps.Find( (MAP_ID)mapID );

    if( it != maps.End() )
        return it->second_;

    return nullptr;
}

void MapManager::NodeRegisterLoadTriggers( Node* node )
{
    if( node )
    {
        PODVector<Node*> result;
        node->GetChildrenWithTag( result, "MapLoadTrigger", true );

        for( unsigned i = 0; i < result.Size(); ++i )
            SubscribeToEvent( result[i], E_GHOST_COLLISION_STARTED, URHO3D_HANDLER( MapManager, HandleLoadTriggerEntered ) );
    }
    else
        URHO3D_LOGERROR( "NodeRegisterLoadTriggers - Invalid Node." );
}

void MapManager::HandleLoadTriggerEntered( StringHash eventType, VariantMap& eventData )
{
    if( SCREEN_TYPE == ScreenType::World )
    {
        auto scene = WORLDSCREEN->GetScene();

        using namespace GhostCollisionBegin;
        Node *node = ((RigidBody*)eventData[P_BODY].GetVoidPtr())->GetNode();
        StringVector tagVec = node->GetTags();
        MapID mapID = MapID::Undefined;
        MapID loadMapID = MapID::Undefined;

        //Get trigger tags
        for( unsigned i = 0; i < tagVec.Size(); ++i )
        {
            if( tagVec[i].StartsWith( "MapID=" ) )
            {
                const unsigned nameLen = String( "MapID=" ).Length();
                mapID = (MapID)(ToInt(tagVec[i].Substring( nameLen, tagVec[i].Length() - nameLen )));
            }
            else if( tagVec[i].StartsWith( "LoadMapID=" ) )
            {
                const unsigned loadLen = String( "LoadMapID=" ).Length();
                loadMapID = (MapID)(ToInt(tagVec[i].Substring( loadLen, tagVec[i].Length() - loadLen )));
            }
        }

        if( mapID != MapID::Undefined && loadMapID != MapID::Undefined )
        {
            const MapID& curMapID = curMap.first_ ? curMap.second_ : MapID::Undefined;
            const MapID& nextMapID = nextMap.first_ ? nextMap.second_ : MapID::Undefined;

            if( curMapID != mapID )
            {
                //Swap Maps Loaded
                if( curMapID == loadMapID && nextMapID == mapID )
                {
                    auto backupMap = curMap;
                    curMap = nextMap;
                    nextMap = backupMap;
                }
                else
                {
                    URHO3D_LOGERROR( "Trigger level and load names out of sequence." );
                }
            }
            else if( nextMapID != loadMapID )
            {
                //Remove any existing level
                if( nextMap.first_ )
                {
                    nextMap.first_->Remove();
                    nextMap.first_ = nullptr;
                    nextMap.second_ = MapID::Undefined;
                }

                //Async loading
                auto baseMap = GetMap( loadMapID );

                if( baseMap )
                {
                    auto mapXML = RESOURCECACHE->GetResource<XMLFile>( baseMap->objectFile );

                    if( mapXML )
                    {
                        SceneResolver resolver;
                        Node *tmpNode = scene->CreateTemporaryChild();

                        if( tmpNode->LoadXML( mapXML->GetRoot(), resolver, false, false, LOCAL ) )
                        {
                            nextMap.first_ = scene->InstantiateXML( mapXML->GetRoot(), baseMap->centerPosition, Quaternion::IDENTITY, LOCAL );
                            nextMap.second_ = baseMap->mapID;

                            //Register triggers from new level node and clear loading flag
                            NodeRegisterLoadTriggers( nextMap.first_ );
                        }
                    }
                    else
                        URHO3D_LOGERROR( "Load level file " + baseMap->objectFile + " not found!" );
                }
            }
        }
    }
}