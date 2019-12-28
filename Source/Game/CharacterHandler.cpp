#include "PrecompiledHeader.h"
#include "CharacterHandler.h"

CharacterHandler::CharacterHandler( Context* context ) :
    HandlerImpl( context ),
	characterNodeID( -1 ),
    character( nullptr ),
    isWalking( false )
{
    SubscribeToEvent( E_UPDATE, URHO3D_HANDLER( CharacterHandler, HandleUpdate ) );
	SubscribeToEvent( E_POSTUPDATE, URHO3D_HANDLER( CharacterHandler, HandlePostUpdate ) );
    SubscribeToEvent( E_MOUSEBUTTONDOWN, URHO3D_HANDLER( CharacterHandler, HandleMouseDown ) );
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
        //Set Camera Position
        CAMERAMANAGER->SetCameraType( CameraType::Follow, characterNode );
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
            controls.Set( Core::CHARACTERCONTROL_Forward, INPUT->GetMouseButtonDown( MOUSEB_LEFT ) || isWalking );
            controls.extraData_["AnimationID"] = -1;
            
            if( INPUT->GetMouseButtonDown( MOUSEB_RIGHT ) )
            {
                auto randomAnim = character->animationMgr->GetAnimationData( Core::AnimationType::Attack );
                controls.extraData_["AnimationID"] = randomAnim->id;
            }

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

                //Load Animation Set
                character->animationMgr->Load( "Models/ani/char/ws.json" );

                //Intercept Network Position
                characterNode->SetInterceptNetworkUpdate( "Network Position", true );
                characterNode->SetInterceptNetworkUpdate( "Network Rotation", true );

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
    if( eventData[MouseButtonDown::P_BUTTON].GetInt() == MOUSEB_LEFT && eventData[MouseButtonDown::P_CLICKS].GetInt() == 2 )
        isWalking = true;
    else
        isWalking = false;
}
