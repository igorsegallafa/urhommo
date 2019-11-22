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
        scene->LoadXML( sceneFile->GetRoot() );
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

    //Add Window for UI
    USERINTERFACE->GetRoot()->AddChild( window );
}
}