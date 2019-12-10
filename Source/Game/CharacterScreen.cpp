#include "PrecompiledHeader.h"
#include "CharacterScreen.h"

CharacterScreen::CharacterScreen( Context* context ) : 
    Screen( context ),
    window( nullptr )
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
    //Load UI Style
    auto style = RESOURCECACHE->GetResource<XMLFile>( "UI/DefaultStyle.xml" );

    //Load Layout from XML
    window = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/CharacterSelect.xml" ), style );

    //Load Window
    window->SetVisible( true );

    //Subscribe Events
    SubscribeToEvent( window->GetChild( "CreateCharacter", true ), E_RELEASED, URHO3D_HANDLER( CharacterScreen, HandleCreateCharacterButtonPressed ) );

    //Add Window for UI
    USERINTERFACE->GetRoot()->AddChild( window );
}

void CharacterScreen::HandleCreateCharacterButtonPressed( StringHash eventType, VariantMap& eventData )
{
    auto lineEditCharacterName = window->GetChildStaticCast<LineEdit>( "CharacterName", true );

    ACCOUNTHANDLER->CreateCharacter( lineEditCharacterName->GetText(), Core::CharacterClass::Pikeman );
}
