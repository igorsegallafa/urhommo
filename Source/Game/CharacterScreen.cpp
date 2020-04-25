#include "PrecompiledHeader.h"
#include "CharacterScreen.h"

CharacterScreen::CharacterScreen( Context* context ) : 
    Screen( context ),
    window_( nullptr ),
    characterClassIndex( 0 ),
    characterSelectedIndex( 0 )
{
}

CharacterScreen::~CharacterScreen()
{
    characters_.Clear();
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

    //Build Character List
    BuildCharacterList();

    //Subscribe Events
    SubscribeToEvent( E_SCREENMODE, URHO3D_HANDLER( CharacterScreen, HandleScreenMode ) );
}

void CharacterScreen::BuildCharacterList()
{
    const auto& characterList = LOGINHANDLER->GetCharacterList();

    if( characterList.Empty() )
    {
        
    }
    else
    {
        int index = 0;
        for( const auto& character : characterList )
        {
            CharacterData characterData;
            characterData.name = character.name;
            characterData.level = character.level;
            characterData.characterClass = character.characterClass;
            characterData.node = nullptr;
            Load( characterData, character.armorId, character.headId, index++ );

            characters_.Push( characterData );
        }
    }
}

void CharacterScreen::Load( CharacterData& characterData, int armorId, int headId, int index )
{
    auto characterListSelected = window_->GetChild( "SelectCharacter", true )->GetChild( "CharacterList", false )->GetChild( ToString("Character%d", index + 1), true );
    if( characterListSelected )
    {
        characterListSelected->GetChildStaticCast<BorderImage>( "CharacterClass", true )->SetTexture( RESOURCECACHE->GetResource<Texture2D>( "Images/CharacterScreen/" + GetCharacterClassName(characterData.characterClass) + ".png" ) );
        characterListSelected->GetChildStaticCast<Text>( "Name", true )->SetText( characterData.name );

        auto xmlFile = RESOURCECACHE->GetResource<XMLFile>( "Objects/Character.xml" );

        if( characterData.node = scene_->InstantiateXML( xmlFile->GetRoot(), Vector3::ZERO, Quaternion::IDENTITY, LOCAL ) )
        {
            auto characterAnimationMgr = characterData.node->GetOrCreateComponent<Core::AnimationEntity>( LOCAL );
            characterAnimationMgr->Load( "Definitions/Animations/" + GetCharacterClassName( characterData.characterClass ) + ".json" );
            characterAnimationMgr->Play( Core::AnimationType::Idle );

            auto characterEquipmentMgr = characterData.node->GetOrCreateComponent<Core::CharacterEquipment>( LOCAL );
            characterEquipmentMgr->SetHead( EQUIPMENTMANAGER->GetEquipModelData( EquipType::Head, characterData.characterClass, headId ) );
            characterEquipmentMgr->SetArmor( EQUIPMENTMANAGER->GetEquipModelData( EquipType::Armor, characterData.characterClass, armorId ) );

            characterData.node->SetEnabledRecursive( false );
        }
    }
}

void CharacterScreen::CreateScene()
{
    auto sceneFile = RESOURCECACHE->GetResource<XMLFile>( "Scenes/CharacterSelect.xml" );

    if( sceneFile )
        scene_->LoadXML( sceneFile->GetRoot() );

    //Create Position Animation
    SharedPtr<ValueAnimation> positionAnimation( new ValueAnimation( context_ ) );
    positionAnimation->SetInterpolationMethod( IM_SPLINE );
    positionAnimation->SetSplineTension( 0.6f );
    positionAnimation->SetKeyFrame( 0.0f, Vector3( 0.f, 5.10297f, 5.5f ) );
    positionAnimation->SetKeyFrame( 1.5f, Vector3( 0.f, 2.14f, 5.5f ) );
    positionAnimation->SetKeyFrame( 3.5f, Vector3( 0.f, 1.14f, 5.5f ) );

    CAMERANODE->SetAttributeAnimation( "Position", positionAnimation, WM_ONCE );
}

void CharacterScreen::SetupViewport()
{
    //Create Viewport and Set it
    SharedPtr<Viewport> viewport( new Viewport( context_, scene_, CAMERA ) );
    RENDERER->SetViewport( 0, viewport );
}

void CharacterScreen::BuildWindow()
{
    //Load Layout from XML
    window_ = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/SelectCharacter.xml" ) );
    window_->SetSize( GRAPHICS->GetSize() );
    window_->GetChild( "Base", true )->SetSize( GRAPHICS->GetSize() );
    window_->GetChild( "CreateCharacter",true )->SetSize( GRAPHICS->GetSize() );
    window_->GetChild( "SelectCharacter", true )->SetSize( GRAPHICS->GetSize() );
    window_->SetVisible( true );

    //Subscribe Events to Select Character Button
    auto characterListSelect = window_->GetChild( "SelectCharacter", true )->GetChild( "CharacterList", false )->GetChild( "Base", false );
    if( characterListSelect )
    {
        int index = 0;
        for( const auto& characterSelectBtn : characterListSelect->GetChildren() )
            SubscribeToEvent( characterSelectBtn, E_RELEASED, std::bind( &CharacterScreen::HandleSelectCharacterButtonPressed, this, index++ ) );
    }

    //Subscribe Events to Select Character Class Button
    auto characterClassListSelect = window_->GetChild( "CreateCharacter", true )->GetChild( "ClassList", false )->GetChild( "Base", false );
    if( characterClassListSelect )
    {
        int index = 0;
        for( const auto& characterSelectBtn : characterClassListSelect->GetChildren() )
            SubscribeToEvent( characterSelectBtn, E_RELEASED, std::bind( &CharacterScreen::HandleSelectCharacterClassButtonPressed, this, index++ ) );
    }

    //Subscribe Events
    SubscribeToEvent( window_->GetChild( "ProceedButton", true ), E_RELEASED, URHO3D_HANDLER( CharacterScreen, HandleProceedButtonPressed ) );
    SubscribeToEvent( window_->GetChild( "BackButton", true ), E_RELEASED, URHO3D_HANDLER( CharacterScreen, HandleBackButtonPressed ) );

    //Add Window for UI
    gui_->AddChild( window_ );
}

void CharacterScreen::HandleScreenMode( StringHash eventType, VariantMap& eventData )
{
    window_->SetSize( GRAPHICS->GetSize() );
    window_->GetChild( "Base", true )->SetSize( GRAPHICS->GetSize() );
    window_->GetChild( "CreateCharacter", true )->SetSize( GRAPHICS->GetSize() );
    window_->GetChild( "SelectCharacter", true )->SetSize( GRAPHICS->GetSize() );
}

void CharacterScreen::HandleSelectCharacterButtonPressed( int index )
{
    if( index >= 0 && index < characters_.Size() )
    {
        auto characterSelected = characters_[index];

        //Set background as select
        auto characterListSelect = window_->GetChild( "SelectCharacter", true )->GetChild( "CharacterList", false )->GetChild( "Base", false );
        if( characterListSelect )
        {
            int curIndex = 0;
            for( const auto& characterSelectBtn : characterListSelect->GetChildren() )
                characterSelectBtn->GetChild( "BackgroundSelected", false )->SetVisible( index == curIndex++ );
        }

        auto characterInfo = window_->GetChild( "SelectCharacter", true )->GetChild( "CharacterInfo", true );
        if( characterInfo )
        {
            characterInfo->GetChildStaticCast<Text>( "NameValue", false )->SetText( characterSelected.name );
            characterInfo->GetChild( "Level", false )->GetChildStaticCast<Text>( "Value", false )->SetText( ToString( "%d", characterSelected.level ) );
            characterInfo->GetChild( "Class", false )->GetChildStaticCast<Text>( "Value", false )->SetText( GetCharacterClassName( characterSelected.characterClass ) );
            characterInfo->GetChild( "Race", false )->GetChildStaticCast<Text>( "Value", false )->SetText( GetCharacterRaceName( GetCharacterClassRace( characterSelected.characterClass ) ) );
            characterInfo->SetVisible( true );
        }

        //Hide other characters
        for( const auto& character : characters_ )
            if( character.node )
                character.node->SetEnabledRecursive( false );

        //Show selected character
        if( characterSelected.node )
        {
            characterSelected.node->SetEnabledRecursive( true );
            characterSelected.node->GetComponent<Core::AnimationEntity>( true )->Play( Core::AnimationType::Attack, true );
            characterSelected.node->GetComponent<Core::AnimationEntity>( true )->Play( Core::AnimationType::Idle );
        }

        characterSelectedIndex = index;
    }
    else
    {
        window_->GetChild( "CreateCharacter", true )->SetVisible( true );
        window_->GetChild( "SelectCharacter", true )->SetVisible( false );
    }
}

void CharacterScreen::HandleSelectCharacterClassButtonPressed( int index )
{
    //Set selected character class index
    characterClassIndex = index;

    //Set background as select
    auto characterClassListSelect = window_->GetChild( "CreateCharacter", true )->GetChild( "ClassList", false )->GetChild( "Base", false );
    if( characterClassListSelect )
    {
        int curIndex = 0;
        for( const auto& characterSelectBtn : characterClassListSelect->GetChildren() )
            characterSelectBtn->GetChild( "Selected", false )->SetVisible( index == curIndex++ );
    }
}

void CharacterScreen::HandleCreateCharacterButtonPressed( StringHash eventType, VariantMap& eventData )
{
    auto lineEditCharacterName = window_->GetChildStaticCast<LineEdit>( "CharacterName", true );

    USERHANDLER->CreateCharacter( lineEditCharacterName->GetText(), CharacterClass::Pikeman );
}

void CharacterScreen::HandleProceedButtonPressed( StringHash eventType, VariantMap& eventData )
{
    if( window_->GetChild( "CreateCharacter", true )->IsVisible() )
    {
        auto lineEditCharacterName = window_->GetChild( "CreateCharacter", true )->GetChild( "CharacterName", true )->GetChildStaticCast<LineEdit>( "Input", true );

        if( lineEditCharacterName->GetText().Length() )
        {
            if( auto characterClassSelected = GetCharacterClassSelected(); characterClassSelected != CharacterClass::Undefined )
                USERHANDLER->CreateCharacter( lineEditCharacterName->GetText(), characterClassSelected );
            else
                NOTIFICATIONTEXT->Push( "Select a valid character class!" );
        }
        else
            NOTIFICATIONTEXT->Push( "Insert a character name!" );
    }
    else
    {
        if( characterSelectedIndex >= 0 && characterSelectedIndex < characters_.Size() )
        {
            auto characterSelected = characters_[characterSelectedIndex];

            USERHANDLER->SelectCharacter( characterSelected.name );
        }
    }
}

void CharacterScreen::HandleBackButtonPressed( StringHash eventType, VariantMap& eventData )
{
    if( window_->GetChild( "CreateCharacter", true )->IsVisible() )
    {
        window_->GetChild( "CreateCharacter", true )->SetVisible( false );
        window_->GetChild( "SelectCharacter", true )->SetVisible( true );
    }
    else
    {
        NETWORKHANDLER->CloseConnections();
        SCREENMANAGER->ChangeScreen( ScreenType::Login );
    }
}

const CharacterClass& CharacterScreen::GetCharacterClassSelected()
{
    switch( characterClassIndex )
    {
        case 0:
            return CharacterClass::Archer;
        case 1:
            return CharacterClass::Fighter;
        case 2:
            return CharacterClass::Mechanician;
        case 3:
            return CharacterClass::Pikeman;
    }

    return CharacterClass::Undefined;
}