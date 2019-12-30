#pragma once

class ChatHandler : public HandlerImpl
{
    URHO3D_OBJECT( ChatHandler, HandlerImpl );
public:
    //! Constructor.
    ChatHandler( Context* context );

    //! Deconstructor.
    ~ChatHandler();

    //! World Data Handler.
    bool HandleChatGame( Connection* connection, MemoryBuffer& message );
};

