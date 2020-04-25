#pragma once

#define MAP_ID   int

enum class MapID
{
    Undefined,

    Ricarten,
    GardenOfFreedom,
    BambooForest,
};

class BaseMap : public Object
{
    URHO3D_OBJECT( BaseMap, Object );
public:
    //! Constructor.
    BaseMap( Context* context ) : Object( context ),
        mapID_(MapID::Undefined),
        centerPosition_(Vector3::ZERO){}

    //! Deconstructor.
    ~BaseMap(){}
public:
    MapID mapID_;
    String name_;
    String objectFile_;
    Vector3 centerPosition_;
};