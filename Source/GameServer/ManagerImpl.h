#pragma once

namespace Manager
{
class Impl : public Object
{
    URHO3D_OBJECT( Impl, Object );
public:
    //! Constructor.
    Impl( Context* context_ ) : Object( context_ ) {}

    //! Deconstructor.
    ~Impl() {}

    //! Initialize Object.
    virtual bool Init() = 0;

    //! UnInitialize Object.
    virtual void UnInit() = 0;
};
}