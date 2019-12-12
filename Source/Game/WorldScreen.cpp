#include "PrecompiledHeader.h"
#include "WorldScreen.h"

WorldScreen::WorldScreen( Context* context ) : Screen( context )
{
}

WorldScreen::~WorldScreen()
{
}

void WorldScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();
}

void WorldScreen::CreateScene()
{
    auto sceneFile = RESOURCECACHE->GetResource<XMLFile>( "Scenes/World.xml" );

    if( sceneFile )
        scene->LoadXML( sceneFile->GetRoot() );

    /*Depracated 
    scene->InstantiateXML( RESOURCECACHE->GetResource<XMLFile>( "Objects/s_v2/s_v2.xml" )->GetRoot(), Vector3::ZERO, Quaternion::IDENTITY, LOCAL );
    auto characterNode = scene->InstantiateXML( RESOURCECACHE->GetResource<XMLFile>( "Objects/char/player_m0.xml" )->GetRoot(), Vector3( -76.9475f, 5.0f, -22.1408f ), Quaternion::IDENTITY, LOCAL );
    CAMERAMANAGER->SetCameraType( CameraType::Follow, characterNode );*/
}

void WorldScreen::SetupViewport()
{
    //Set to doesn't render the Hit Boxes
    CAMERA->SetViewMask( DEFAULT_VIEWMASK & ~8 );

    //Create Viewport and Set it
    SharedPtr<Viewport> viewport( new Viewport( context_, scene, CAMERA ) );
    RENDERER->SetViewport( 0, viewport );
}

void WorldScreen::BuildWindow()
{
}