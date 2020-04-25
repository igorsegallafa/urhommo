#pragma once

class Server : public Application
{
public:
    //! Constructor.
    Server( Context* context );

    void Setup();

    void Start();

    void Stop();
private:
    void PrintSignature();
};

URHO3D_DEFINE_APPLICATION_MAIN( Server )