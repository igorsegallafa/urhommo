#pragma once

class ManagerImpl : public Object
{
    URHO3D_OBJECT( ManagerImpl, Object );
public:
    //! Constructor.
    ManagerImpl( Context* context ) : Object( context ) {}

    //! Deconstructor.
    ~ManagerImpl() {}

    //! Initialize Object.
    virtual bool Init() { return false; }

    //! UnInitialize Object.
    virtual void UnInit() {}
};