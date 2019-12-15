#pragma once

#define MAP_ID   int

enum class MapID
{
    Undefined,

    Ricarten,
    GardenOfFreedom,
};

class Map : public Object
{
    URHO3D_OBJECT( Map, Object );
public:
    //! Constructor.
    Map( Context* context ) : Object( context ),
        mapID(MapID::Undefined),
        centerPosition(Vector3::ZERO),
        scene(nullptr){}

    //! Deconstructor.
    ~Map(){}
public:
    MapID mapID;
    String name;
    String objectFile;
    Vector3 centerPosition;
    SharedPtr<Scene> scene;
};