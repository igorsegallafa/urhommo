#pragma once

class Server : public Application
{
public:
    //! Constructor.
    Server( Context* context );

    void Setup();

    void Start();

    void Stop();
};

URHO3D_DEFINE_APPLICATION_MAIN( Server )