#pragma once

struct NotificationTextData
{
    SharedPtr<Text> text_;
    float elapedTime_;
    bool fadingOut_;
};

class NotificationText : public Object
{
    URHO3D_OBJECT( NotificationText, Object );
public:
    //! Constructor.
    NotificationText( Context* context );

    //! Deconstructor.
    ~NotificationText();

    //! Create Notification Text.
    void Push( const String& message );

    //! Clear Notifications.
    void Clear();

    //! Reset Notifications.
    void Reset();

    //! Register Object Factory.
    static void RegisterObject( Context* context );
private:
    //! Update Handler.
    void HandleUpdate( StringHash eventType, VariantMap& eventData );

    //! Screen Mode Handler.
    void HandleScreenMode( StringHash eventType, VariantMap& eventData );
private:
    SharedPtr<UIElement> gui_;
    Vector<NotificationTextData> notifications_;
};