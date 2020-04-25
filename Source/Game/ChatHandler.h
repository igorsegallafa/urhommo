#pragma once

class ChatHandler : public HandlerImpl
{
    URHO3D_OBJECT( ChatHandler, HandlerImpl );
public:
    //! Constructor.
    ChatHandler( Context* context );

    //! Deconstructor.
    ~ChatHandler();

    //! Initialize Handler.
    bool Init() override;

    //! Load.
    void Load();

    //! Add Message to Chat.
    void AddMessage( const String& message );

    //! Chat Game Handler.
    bool HandleChatGame( Connection* connection, MemoryBuffer& message );
private:
    //! Send Message for Server.
    void SendMessage( const String& message );

    //! Key Down Handler.
    void HandleKeyDown( StringHash eventType, VariantMap& eventData );

    //! Send Button Handler.
    void HandleSendButtonPressed( StringHash eventType, VariantMap& eventData );
private:
    SharedPtr<UIElement> chatWindow_;
};

