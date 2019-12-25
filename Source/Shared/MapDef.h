#pragma once

#define MAP_ID   int

enum class MapID
{
    Undefined,

    Ricarten,
    GardenOfFreedom,
};

class BaseMap : public Object
{
    URHO3D_OBJECT( BaseMap, Object );
public:
    //! Constructor.
    BaseMap( Context* context ) : Object( context ),
        mapID(MapID::Undefined),
        centerPosition(Vector3::ZERO),
        scene(nullptr){}

    //! Deconstructor.
    ~BaseMap(){}
public:
    MapID mapID;
    String name;
    String objectFile;
    Vector3 centerPosition;
    SharedPtr<Scene> scene;
};