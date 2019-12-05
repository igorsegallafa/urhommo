#include "PrecompiledHeader.h"
#include "CharacterScreen.h"

CharacterScreen::CharacterScreen( Context* context ) : Screen( context )
{
}

CharacterScreen::~CharacterScreen()
{
}

void CharacterScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();
}

void CharacterScreen::CreateScene()
{
    auto sceneFile = RESOURCECACHE->GetResource<XMLFile>( "Scenes/CharacterSelect.xml" );

    if( sceneFile )
        scene->LoadXML( sceneFile->GetRoot() );
}

void CharacterScreen::SetupViewport()
{
    //Create Viewport and Set it
    SharedPtr<Viewport> viewport( new Viewport( context_, scene, CAMERA ) );
    RENDERER->SetViewport( 0, viewport );
}

void CharacterScreen::BuildWindow()
{
}