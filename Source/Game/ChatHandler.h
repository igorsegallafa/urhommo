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
private:
    SharedPtr<UIElement> chatWindow;
};

