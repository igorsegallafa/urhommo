#pragma once

class Game : public Application
{
public:
    //! Constructor.
    Game( Context* context );

    //! Deconstructor.
    ~Game();

    //! Setup Engine.
    void Setup();

    //! Start Engine.
    void Start();

    //! Stop Engine.
    void Stop();
};

URHO3D_DEFINE_APPLICATION_MAIN( Game )