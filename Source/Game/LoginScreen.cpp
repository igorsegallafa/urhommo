#include "PrecompiledHeader.h"
#include "LoginScreen.h"

namespace Core
{
LoginScreen::LoginScreen( Context* context ) : 
    Screen( context ),
    window( nullptr )
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
}

void LoginScreen::CreateScene()
{
    auto sceneFile = RESOURCECACHE->GetResource<XMLFile>( "Scenes/Login.xml" );

    if( sceneFile )
    {
        scene->LoadXML( sceneFile->GetRoot() );

        if( auto birdNode = scene->GetChild( "s_login_niao", true ); birdNode )
        {
            if( auto animCtrl = birdNode->GetComponent<AnimationController>(); animCtrl )
                animCtrl->PlayExclusive( "Models/login/s_login_niao_Take.ani", 0, true );
        }
    }
}

void LoginScreen::SetupViewport()
{
    //Create Viewport and Set it
    SharedPtr<Viewport> viewport( new Viewport( context_, scene, CAMERA ) );
    RENDERER->SetViewport( 0, viewport );
}

void LoginScreen::BuildWindow()
{
    //Load UI Style
    auto style = RESOURCECACHE->GetResource<XMLFile>( "UI/DefaultStyle.xml" );

    //Load Layout from XML
    window = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/Login.xml" ), style );

    //Subscribe Events
    SubscribeToEvent( window->GetChild( "ButtonLogin", true ), E_RELEASED, URHO3D_HANDLER( LoginScreen, HandleLoginButtonPressed ) );

    //Add Window for UI
    USERINTERFACE->GetRoot()->AddChild( window );
}

void LoginScreen::HandleLoginButtonPressed( StringHash eventType, VariantMap& eventData )
{
    auto lineEditAccount = window->GetChildStaticCast<LineEdit>( "Account", true );
    auto lineEditPassword = window->GetChildStaticCast<LineEdit>( "Password", true );

    LOGINHANDLER->ProcessLogin( lineEditAccount->GetText(), lineEditPassword->GetText() );
}
}