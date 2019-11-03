#pragma once

class Main : public Application
{
public:
    //! Constructor.
    Main( Context* context );

    void Setup();

    void Start();

    void Stop();
};

URHO3D_DEFINE_APPLICATION_MAIN( Main )