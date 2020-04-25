#include "PrecompiledHeader.h"
#include "ScreenManager.h"

ScreenManager::ScreenManager( Context* context ) : 
    ManagerImpl( context ),
    activeScreenType_( ScreenType::Login ),
    fadeStep_( FadeStep::Prepare ),
    fadeTime_( 0.f )
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
    ChangeScreen( ScreenType::Login );
    return true;
}

void ScreenManager::UnInit()
{
}

void ScreenManager::ChangeScreen( const ScreenType& screen )
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
    fadeTime_ -= time;

    //Prepare to fade out
    if( fadeStep_ == FadeStep::Prepare )
    {
        //No active screen?
        if( !activeScreen_ )
            fadeStep_ = FadeStep::Out;
        else
        {
            BuildWindow();

            if( window_ )
                window_->SetOpacity( 0.0f );

            fadeTime_ = MAX_FADE_TIME;
            fadeStep_ = FadeStep::Out;
        }
    }
    else if( fadeStep_ == FadeStep::Out )
    {
        //No active screen?
        if( !activeScreen_ )
            fadeStep_ = FadeStep::ReleaseScreen;
        else
        {
            window_->SetOpacity( 1.0f - fadeTime_ / MAX_FADE_TIME );

            //Fade time its over? So go to the next step
            if( fadeTime_ <= 0.0f )
                fadeStep_ = FadeStep::ReleaseScreen;
        }
    }
    else if( fadeStep_ == FadeStep::ReleaseScreen )
    {
        //No active screen?
        if( !activeScreen_ )
            fadeStep_ = FadeStep::CreateScreen;
        else
        {
            //We can not create new screen here, or it may cause errors, we have to create it at the next update point
            activeScreen_ = SharedPtr<Screen>();
            fadeStep_ = FadeStep::CreateScreen;
        }
    }
    else if( fadeStep_ == FadeStep::CreateScreen )
    {
        //Create new Screen
        switch( screenQueue_.Front() )
        {
            case ScreenType::World:
                activeScreen_ = new WorldScreen( context_ );
                break;
            case ScreenType::Test:
                activeScreen_ = new TestScreen( context_ );
                break;
            case ScreenType::Login:
                activeScreen_ = new LoginScreen( context_ );
                break;
            case ScreenType::Character:
                activeScreen_ = new CharacterScreen( context_ );
                break;
        }

        activeScreen_->Init();
        activeScreenType_ = screenQueue_.Front();

        //Remove the old fade layer
        if( window_ )
            window_->Remove();

        //Add a new fade layer
        BuildWindow();

        if( window_ )
            window_->SetOpacity( 1.0f );

        fadeTime_ = MAX_FADE_TIME;
        fadeStep_ = FadeStep::In;
    }
    else if( fadeStep_ == FadeStep::In )
    {
        if( window_ )
            window_->SetOpacity( fadeTime_ / MAX_FADE_TIME );

        //Fade time its over? So go to the next step
        if( fadeTime_ <= 0.0f )
            fadeStep_ = FadeStep::Finished;
    }
    else if( fadeStep_ == FadeStep::Finished )
    {
        //Remove fade layer
        if( window_ )
        {
            window_->Remove();
            window_ = SharedPtr<Window>();
        }

        //Remove the task
        screenQueue_.PopFront();

        //Release all unused resources
        RESOURCECACHE->ReleaseAllResources( false );
    }
}

void ScreenManager::HandleSetLevelQueue( StringHash eventType, VariantMap& eventData )
{
    //Busy now
    if( screenQueue_.Size() )
        return;

    //Push to queue
    screenQueue_.Push( (ScreenType)eventData[E_SET_SCREEN].GetInt() );

    //Init fade status
    fadeStep_ = FadeStep::Prepare;
}

void ScreenManager::HandleResolutionChange( StringHash eventType, VariantMap& eventData )
{
}

void ScreenManager::BuildWindow()
{
    window_ = new Window( context_ );

    //Make the window and setup it
    GetSubsystem<UI>()->GetRoot()->AddChild( window_ );
    window_->SetSize( GetSubsystem<Graphics>()->GetWidth(), GetSubsystem<Graphics>()->GetHeight() );
    window_->SetLayout( LM_FREE );
    window_->SetAlignment( HA_CENTER, VA_CENTER );
    window_->SetColor( Color( 0.0f, 0.0f, 0.0f, 1.0f ) );
    window_->BringToFront();
}