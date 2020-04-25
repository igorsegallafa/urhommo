#pragma once

const float MAX_FADE_TIME = 1.0f;
static const StringHash E_SET_SCREEN = StringHash( "Set Screen" );

enum class ScreenType
{
    None,

    Login,
    Character,
    World,
    Test,
};

class Screen : public Object
{
    URHO3D_OBJECT( Screen, Object );
public:
    //! Constructor.
    Screen( Context* context ) : Object( context ), scene_( nullptr ), gui_( nullptr ){}

    //! Deconstructor.
    ~Screen() { Dispose(); }

    //! Initialize Screen.
    virtual void Init();

    //! Getters.
    Scene* GetScene(){ return scene_; }
private:
    //! Screen Mode Handler.
    void HandleScreenMode( StringHash eventType, VariantMap& eventData );
protected:
    virtual void Run();
    virtual void Pause();
    virtual void Dispose();
protected:
    SharedPtr<UIElement> gui_;    //!< Pointer for GUI.
    SharedPtr<Scene> scene_; //!< Pointer for scene.
};