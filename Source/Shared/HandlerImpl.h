#pragma once

class HandlerImpl : public Object
{
    URHO3D_OBJECT( HandlerImpl, Object );
public:
    //! Constructor.
    HandlerImpl( Context* context ) : Object( context ) {}

    //! Deconstructor.
    ~HandlerImpl() {}

    //! Initialize Object.
    virtual bool Init() { return false; }

    //! UnInitialize Object.
    virtual void UnInit() {}
};