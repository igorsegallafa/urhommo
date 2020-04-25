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

    //! Post Update Handler.
    void HandlePostUpdate( StringHash eventType, VariantMap& eventData );

    //! Render GUI Handler.
    void HandleRender( StringHash eventType, VariantMap& eventData );

    //! Window Resized Handler.
    void HandleWindowResized( StringHash eventType, VariantMap& eventData );

    //! Mouse Button Down Handler.
    void HandleMouseButtonDown( StringHash eventType, VariantMap& eventData );

    //! Mouse Button Up Handler.
    void HandleMouseButtonUp( StringHash eventType, VariantMap& eventData );

    //! Mouse Move Handler.
    void HandleMouseMove( StringHash eventType, VariantMap& eventData );

    //! Mouse Wheel Handler.
    void HandleMouseWheel( StringHash eventType, VariantMap& eventData );

    //! Key Down Handler.
    void HandleKeyDown( StringHash eventType, VariantMap& eventData );

    //! Key Up Handler.
    void HandleKeyUp( StringHash eventType, VariantMap& eventData );

    //! Key Char Handler.
    void HandleKeyChar( StringHash eventType, VariantMap& eventData );

    //! ImGui New Frame Handler.
    void HandleImGuiNewFrame( StringHash eventType, VariantMap& eventData );
private:
    SharedPtr<imgui> imGui_; //!< ImGui.
};

URHO3D_DEFINE_APPLICATION_MAIN( Game )