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
    Screen( Context* context ) : Object( context ), scene( nullptr ) {}

    //! Deconstructor.
    ~Screen() { Dispose(); }

    //! Initialize Screen.
    virtual void Init();

    //! Getters.
    Scene* GetScene(){ return scene; }
protected:
    virtual void Run();
    virtual void Pause();
    virtual void Dispose();
protected:
    SharedPtr<Scene> scene; //!< Pointer for scene.
};