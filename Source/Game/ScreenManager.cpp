#include "PrecompiledHeader.h"
#include "ScreenManager.h"

ScreenManager::ScreenManager( Context* context ) : ManagerImpl( context )
{
    context->RegisterFactory<LoginScreen>( "Screen" );
    context->RegisterFactory<CharacterScreen>( "Screen" );
    context->RegisterFactory<TestScreen>( "Screen" );
    context->RegisterFactory<WorldScreen>( "Screen" );

    SubscribeToEvent( E_SET_SCREEN, URHO3D_HANDLER( ScreenManager, HandleSetLevelQueue ) );
    SubscribeToEvent( E_SCREENMODE, URHO3D_HANDLER( ScreenManager, HandleResolutionChange ) );
    SubscribeToEvent( E_UPDATE, URHO3D_HANDLER( ScreenManager, HandleUpdate ) );
}

ScreenManager::~ScreenManager()
{
}

bool ScreenManager::Init()
{
    SetActiveScreen( ScreenType::Login );
    return true;
}

void ScreenManager::UnInit()
{
}

void ScreenManager::SetActiveScreen( const ScreenType& screen )
{
    VariantMap& eventData = GetEventDataMap();
    eventData[E_SET_SCREEN] = (int)screen;
    SendEvent( E_SET_SCREEN, eventData );
}

void ScreenManager::HandleUpdate( StringHash eventType, VariantMap& eventData )
{
    //Recover the current time step
    float time = eventData[Update::P_TIMESTEP].GetFloat();

    //Move sprites, scale movement with time step
    fadeTime -= time;

    //Prepare to fade out
    if( fadeStep == FadeStep::Prepare )
    {
        //No active screen?
        if( !activeScreen )
            fadeStep = FadeStep::Out;
        else
        {
            BuildWindow();

            if( window )
                window->SetOpacity( 0.0f );

            fadeTime = MAX_FADE_TIME;
            fadeStep = FadeStep::Out;
        }
    }
    else if( fadeStep == FadeStep::Out )
    {
        //No active screen?
        if( !activeScreen )
            fadeStep = FadeStep::ReleaseScreen;
        else
        {
            window->SetOpacity( 1.0f - fadeTime / MAX_FADE_TIME );

            //Fade time its over? So go to the next step
            if( fadeTime <= 0.0f )
                fadeStep = FadeStep::ReleaseScreen;
        }
    }
    else if( fadeStep == FadeStep::ReleaseScreen )
    {
        //No active screen?
        if( !activeScreen )
            fadeStep = FadeStep::CreateScreen;
        else
        {
            //We can not create new screen here, or it may cause errors, we have to create it at the next update point
            activeScreen = SharedPtr<Screen>();
            fadeStep = FadeStep::CreateScreen;
        }
    }
    else if( fadeStep == FadeStep::CreateScreen )
    {
        //Create new Screen
        switch( screenQueue.Front() )
        {
            case ScreenType::World:
                activeScreen = new WorldScreen( context_ );
                break;
            case ScreenType::Test:
                activeScreen = new TestScreen( context_ );
                break;
            case ScreenType::Login:
                activeScreen = new LoginScreen( context_ );
                break;
            case ScreenType::Character:
                activeScreen = new CharacterScreen( context_ );
                break;
        }

        activeScreen->Init();
        activeScreenType = screenQueue.Front();

        //Remove the old fade layer
        if( window )
            window->Remove();

        //Add a new fade layer
        BuildWindow();

        if( window )
            window->SetOpacity( 1.0f );

        fadeTime = MAX_FADE_TIME;
        fadeStep = FadeStep::In;
    }
    else if( fadeStep == FadeStep::In )
    {
        if( window )
            window->SetOpacity( fadeTime / MAX_FADE_TIME );

        //Fade time its over? So go to the next step
        if( fadeTime <= 0.0f )
            fadeStep = FadeStep::Finished;
    }
    else if( fadeStep == FadeStep::Finished )
    {
        //Remove fade layer
        if( window )
        {
            window->Remove();
            window = SharedPtr<Window>();
        }

        //Unsubscribe update event
        UnsubscribeFromEvent( E_UPDATE );

        //Remove the task
        screenQueue.PopFront();

        //Release all unused resources
        RESOURCECACHE->ReleaseAllResources( false );
    }
}

void ScreenManager::HandleSetLevelQueue( StringHash eventType, VariantMap& eventData )
{
    //Busy now
    if( screenQueue.Size() )
        return;

    //Push to queue
    screenQueue.Push( (ScreenType)eventData[E_SET_SCREEN].GetInt() );

    //Init fade status
    fadeStep = FadeStep::Prepare;
}

void ScreenManager::HandleResolutionChange( StringHash eventType, VariantMap& eventData )
{
}

void ScreenManager::BuildWindow()
{
    window = new Window( context_ );

    //Make the window and setup it
    GetSubsystem<UI>()->GetRoot()->AddChild( window );
    window->SetSize( GetSubsystem<Graphics>()->GetWidth(), GetSubsystem<Graphics>()->GetHeight() );
    window->SetLayout( LM_FREE );
    window->SetAlignment( HA_CENTER, VA_CENTER );
    window->SetColor( Color( 0.0f, 0.0f, 0.0f, 1.0f ) );
    window->BringToFront();
}