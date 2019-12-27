#include "PrecompiledHeader.h"
#include "WorldScreen.h"

WorldScreen::WorldScreen( Context* context ) : Screen( context )
{
    SubscribeToEvent( E_POSTRENDERUPDATE, URHO3D_HANDLER( WorldScreen, HandlePostRenderUpdate ) );
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
    //Set Game Server Connection Scene
    CONNECTIONG->SetScene( scene );

    //Create Scene Components
    scene->CreateComponent<Octree>( LOCAL );
    scene->CreateComponent<PhysicsWorld>( LOCAL );

    //Game Server can Set Connection Scene
    CONNECTIONG->Send( MSGID_WorldData, true, true, VectorBuffer() );
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

void WorldScreen::HandlePostRenderUpdate( StringHash eventType, VariantMap & eventData )
{
    auto debugRenderer = scene->GetOrCreateComponent<DebugRenderer>();

    if( debugRenderer )
        scene->GetComponent<PhysicsWorld>()->DrawDebugGeometry( debugRenderer, true );
}