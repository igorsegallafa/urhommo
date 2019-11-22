#pragma once

class Game : public Application
{
public:
    //! Constructor.
    Game( Context* context );

    void Setup();

    void Start();

    void Stop();
};

URHO3D_DEFINE_APPLICATION_MAIN( Game )