#pragma once

namespace Handler
{
class Network : public Object
{
    URHO3D_OBJECT( Network, Object );
public:
    //! Constructor.
    Network( Context* context_ );

    //! Deconstructor.
    ~Network();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();
private:
    //! Handle Client Connection Identity.
    void HandleClientIdentity( StringHash eventType, VariantMap& eventData );

    //! Handle New Client Connection.
    void HandleClientConnected( StringHash eventType, VariantMap& eventData );

    //! Handle Client Disconnection.
    void HandleClientDisconnected( StringHash eventType, VariantMap& eventData );
};
}