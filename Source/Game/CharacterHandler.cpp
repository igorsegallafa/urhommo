#include "PrecompiledHeader.h"
#include "CharacterHandler.h"

CharacterHandler::CharacterHandler( Context* context ) :
    HandlerImpl( context ),
	characterNodeID( -1 ),
    character( nullptr ),
    isWalking( false ),
    animationToSet( 0 )
{
    SubscribeToEvent( E_UPDATE, URHO3D_HANDLER( CharacterHandler, HandleUpdate ) );
	SubscribeToEvent( E_POSTUPDATE, URHO3D_HANDLER( CharacterHandler, HandlePostUpdate ) );
    SubscribeToEvent( E_MOUSEBUTTONDOWN, URHO3D_HANDLER( CharacterHandler, HandleMouseDown ) );
    SubscribeToEvent( E_NETWORKUPDATESENT, URHO3D_HANDLER( CharacterHandler, HandleNetworkUpdateSent ) );
}

CharacterHandler::~CharacterHandler()
{
    if( character )
    {
        character->Remove();
        character = nullptr;
    }
}

bool CharacterHandler::HandleWorldData( Connection* connection, MemoryBuffer& message )
{
	//Set Character Node ID
	characterNodeID = message.ReadInt();
    mapIDToLoad = (MapID)message.ReadInt();

	//Change to World Screen
	SCREENMANAGER->ChangeScreen(ScreenType::World);

    return true;
}

void CharacterHandler::LoadCharacter()
{
    auto characterNode = character->GetNode();

    if( characterNode )
    {
        //Load Animation Set
        character->animationMgr->Load( "Models/ani/char/ws.json" );

        //Create Character Crowd Agent
        auto crowdAgent = characterNode->CreateComponent<CrowdAgent>();
        if( crowdAgent )
        {
            crowdAgent->SetHeight( 1.7f );
            crowdAgent->SetRadius( 1.f );
            crowdAgent->SetMaxSpeed( 1.0f );
            crowdAgent->SetUpdateNodePosition( false );
        }

        //Set Camera Position
        CAMERAMANAGER->SetCameraType( CameraType::Follow, characterNode );
    }
}

void CharacterHandler::ChangeAnimation( const Core::AnimationType& animationType )
{
    if( character )
    {
        auto animationData = character->animationMgr->GetAnimationData( animationType );

        if( animationData )
            ChangeAnimation( animationData->id );
    }
}

void CharacterHandler::HandleUpdate( StringHash eventType, VariantMap& eventData )
{
    if( SCREEN_TYPE == ScreenType::World && character && CONNECTIONG )
    {
        auto characterNode = character->GetNode();

        if( characterNode )
        {
            //Set Connection Controls
            Controls controls;
            controls.yaw_ = CAMERAMANAGER->GetCameraYaw() + CAMERAMANAGER->GetMouseYaw();
            controls.extraData_["AnimationID"] = -1;
            
            if( !USERINTERFACE->GetFocusElement() )
                controls.Set( Core::CHARACTERCONTROL_Forward, INPUT->GetMouseButtonDown( MOUSEB_LEFT ) || isWalking );

            if( INPUT->GetMouseButtonDown( MOUSEB_RIGHT ) )
            {
                auto test = character->animationMgr->GetAnimationData( Core::AnimationType::Attack );
                
                if( test )
                    animationToSet = test->id;
            }
            
            //Have animation to set?
            if( animationToSet != 0 )
                controls.extraData_["AnimationID"] = animationToSet;

            CONNECTIONG->SetPosition( characterNode->GetPosition() );
            CONNECTIONG->SetRotation( characterNode->GetRotation() );
            CONNECTIONG->SetControls( controls );
        }
    }
}

void CharacterHandler::HandlePostUpdate( StringHash eventType, VariantMap& eventData )
{
	if( SCREEN_TYPE == ScreenType::World )
	{
        //Look for Character Component
        if( character == nullptr )
        {
            auto characterNode = ACTIVESCREEN->GetScene()->GetNode( characterNodeID );

            //Character Node Found!
            if( characterNode )
            {
                character = characterNode->GetComponent<Core::Character>( true );
                character->CreatePhysicsComponent();
                character->connection = CONNECTIONG;
                character->animationMgr = characterNode->GetComponent<Core::AnimationEntity>( true );

                //Intercept Network Position
                characterNode->SetInterceptNetworkUpdate( "Network Position", true );
                characterNode->SetInterceptNetworkUpdate( "Network Rotation", true );
                
                //Disabled: characterNode->GetComponent<AnimationController>( true )->SetInterceptNetworkUpdate( "Network Animations", true );

                //Load Map
                MAPMANAGER->Load( mapIDToLoad );

                //Load Character
                LoadCharacter();
            }
        }
	}
}

void CharacterHandler::HandleMouseDown( StringHash eventType, VariantMap& eventData )
{
    //Character isn't created yet
    if( character == nullptr )
        return;

    //Get Dynamic Navigation Mesh Pointer
    auto navigationMesh = character->GetNode()->GetScene()->GetComponent<NavigationMesh>( true );

    //Has Dynamic Navigation Mesh?
    if( navigationMesh )
    {
        selectedNode = CAMERAMANAGER->GetNodeRaycast();

        //Self selecting? Ignore it!
        if( selectedNode == (character ? character->GetNode() : nullptr) )
            selectedNode = nullptr;

        //Found Selected Node?
        if( selectedNode )
        {
            Vector3 pathPos = navigationMesh->FindNearestPoint( selectedNode->GetPosition(), Vector3( 1.0f, 1.0f, 1.0f ) );
            character->SetTargetPosition( pathPos );
        }
        else
            character->ResetTargetPosition();
    }

    if( eventData[MouseButtonDown::P_BUTTON].GetInt() == MOUSEB_LEFT && eventData[MouseButtonDown::P_CLICKS].GetInt() == 2 )
        isWalking = true;
    else
        isWalking = false;
}

void CharacterHandler::HandleNetworkUpdateSent( StringHash eventType, VariantMap & eventData )
{
    animationToSet = 0;
}
