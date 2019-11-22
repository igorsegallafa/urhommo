#include "PrecompiledHeader.h"
#include "ScreenManager.h"

namespace Manager
{
Screen::Screen( Context* context ) : Impl( context )
{
    context->RegisterFactory<Core::LoginScreen>( "Screen" );
    context->RegisterFactory<Core::CharacterScreen>( "Screen" );
    context->RegisterFactory<Core::TestScreen>( "Screen" );
    context->RegisterFactory<Core::WorldScreen>( "Screen" );

    SubscribeToEvent( Core::E_SET_SCREEN, URHO3D_HANDLER( Screen, HandleSetLevelQueue ) );
    SubscribeToEvent( E_SCREENMODE, URHO3D_HANDLER( Screen, HandleResolutionChange ) );
    SubscribeToEvent( E_UPDATE, URHO3D_HANDLER( Screen, HandleUpdate ) );
}

Screen::~Screen()
{
}

bool Screen::Init()
{
    SetActiveScreen( Core::ScreenType::Login );
    return true;
}

void Screen::UnInit()
{
}

void Screen::SetActiveScreen( const Core::ScreenType& screen )
{
    VariantMap& eventData = GetEventDataMap();
    eventData[Core::E_SET_SCREEN] = (int)screen;
    SendEvent( Core::E_SET_SCREEN, eventData );
}

void Screen::HandleUpdate( StringHash eventType, VariantMap& eventData )
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

            fadeTime = Core::MAX_FADE_TIME;
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
            window->SetOpacity( 1.0f - fadeTime / Core::MAX_FADE_TIME );

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
            activeScreen = SharedPtr<Core::Screen>();
            fadeStep = FadeStep::CreateScreen;
        }
    }
    else if( fadeStep == FadeStep::CreateScreen )
    {
        //Create new Screen
        switch( screenQueue.Front() )
        {
            case Core::ScreenType::World:
                activeScreen = new Core::WorldScreen( context_ );
                break;
            case Core::ScreenType::Test:
                activeScreen = new Core::TestScreen( context_ );
                break;
            case Core::ScreenType::Login:
                activeScreen = new Core::LoginScreen( context_ );
                break;
            case Core::ScreenType::Character:
                activeScreen = new Core::CharacterScreen( context_ );
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

        fadeTime = Core::MAX_FADE_TIME;
        fadeStep = FadeStep::In;
    }
    else if( fadeStep == FadeStep::In )
    {
        if( window )
            window->SetOpacity( fadeTime / Core::MAX_FADE_TIME );

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

void Screen::HandleSetLevelQueue( StringHash eventType, VariantMap& eventData )
{
    //Busy now
    if( screenQueue.Size() )
        return;

    //Push to queue
    screenQueue.Push( (Core::ScreenType)eventData[Core::E_SET_SCREEN].GetInt() );

    //Init fade status
    fadeStep = FadeStep::Prepare;
}

void Screen::HandleResolutionChange( StringHash eventType, VariantMap& eventData )
{
}

void Screen::BuildWindow()
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
}