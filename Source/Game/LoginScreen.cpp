#include "PrecompiledHeader.h"
#include "LoginScreen.h"

#include <Urho3D/Urho2D/SpriteSheet2D.h>

LoginScreen::LoginScreen( Context* context ) : 
    Screen( context ),
    loginWindow_( nullptr ),
    worldServerWindow_( nullptr )
{
}

LoginScreen::~LoginScreen()
{
}

void LoginScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();

    //Subscribe Events
    SubscribeToEvent( E_SCREENMODE, URHO3D_HANDLER( LoginScreen, HandleScreenMode ) );
}

void LoginScreen::CreateScene()
{
    auto sceneFile = RESOURCECACHE->GetResource<XMLFile>( "Scenes/CharacterSelect.xml" );

    if( sceneFile )
        scene_->LoadXML( sceneFile->GetRoot() );
}

void LoginScreen::SetupViewport()
{
    //Create Viewport and Set it
    SharedPtr<Viewport> viewport( new Viewport( context_, scene_, CAMERA ) );
    RENDERER->SetViewport( 0, viewport );
}

void LoginScreen::BuildWindow()
{
    worldServerWindow_ = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/LoginGameServer.xml" ) );

    loginWindow_ = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/Login.xml" ) );
    loginWindow_->SetSize( GRAPHICS->GetSize() );
    loginWindow_->GetChild( "LoginTip", true )->SetWidth( GRAPHICS->GetWidth() );

    //Load Window
    loginWindow_->SetVisible( true );
    worldServerWindow_->SetVisible( false );

    //Subscribe Events
    SubscribeToEvent( loginWindow_->GetChild( "ButtonLogin", true ), E_RELEASED, URHO3D_HANDLER( LoginScreen, HandleLoginButtonPressed ) );

    //Add Window for UI
    gui_->AddChild( loginWindow_ );
    gui_->AddChild( worldServerWindow_ );
}

void LoginScreen::HandleScreenMode( StringHash eventType, VariantMap& eventData )
{
    loginWindow_->SetSize( GRAPHICS->GetSize() );
    loginWindow_->GetChild( "LoginTip", true )->SetWidth( GRAPHICS->GetWidth() );
}

void LoginScreen::SetWorldServerList( const Vector<String>& gameServerList )
{
    if( worldServerWindow_ && loginWindow_ )
    {
        worldServerWindow_->RemoveAllChildren();

        int totalServers = 0;
        for( const auto gameServerName : gameServerList )
        {
            auto gameServer = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/LoginThreadItem.xml" ) );
            gameServer->SetPosition( IntVector2( 0, totalServers * 60 ) );
            gameServer->GetChildDynamicCast<Text>( "Name", true )->SetText( gameServerName );
            SubscribeToEvent( gameServer, E_PRESSED, std::bind( &LoginScreen::HandleWorldServerPressed, this, totalServers ) );
            worldServerWindow_->AddChild( gameServer );

            totalServers++;
        }

        loginWindow_->GetChild( "LoginAccount", true )->SetVisible( false );
        worldServerWindow_->SetVisible( true );
    }
}

void LoginScreen::HandleWorldServerPressed( int serverIndex )
{
    //We must to connect to all servers before to send/receive any message
    LOGINHANDLER->ProcessWorldServer( serverIndex );
}

void LoginScreen::HandleLoginButtonPressed( StringHash eventType, VariantMap& eventData )
{
    auto account = loginWindow_->GetChildStaticCast<LineEdit>( "Account", true )->GetText();
    auto password = loginWindow_->GetChildStaticCast<LineEdit>( "Password", true )->GetText();

    if( account.Empty() )
        NOTIFICATIONTEXT->Push( "Insert an account to proceed..." );
    else if( password.Empty() )
        NOTIFICATIONTEXT->Push( "Insert a password to proceed..." );
    else
    {
        NOTIFICATIONTEXT->Push( "Connecting..." );
        LOGINHANDLER->ProcessLogin( account, password );
    }
}