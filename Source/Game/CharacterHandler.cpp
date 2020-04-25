#include "PrecompiledHeader.h"
#include "CharacterHandler.h"

CharacterHandler::CharacterHandler( Context* context ) :
    HandlerImpl( context ),
	characterNodeID_( -1 ),
    character_( nullptr ),
    isWalking_( false ),
    animationToSet_( 0, false ),
    selectedNode_( nullptr ),
    hoveredNode_( nullptr ),
    mapIDToLoad_( MapID::Undefined )
{
    SubscribeToEvent( E_UPDATE, URHO3D_HANDLER( CharacterHandler, HandleUpdate ) );
	SubscribeToEvent( E_POSTUPDATE, URHO3D_HANDLER( CharacterHandler, HandlePostUpdate ) );
    SubscribeToEvent( E_MOUSEBUTTONDOWN, URHO3D_HANDLER( CharacterHandler, HandleMouseDown ) );
    SubscribeToEvent( E_NETWORKUPDATESENT, URHO3D_HANDLER( CharacterHandler, HandleNetworkUpdateSent ) );
}

CharacterHandler::~CharacterHandler()
{
    if( character_ )
    {
        character_->Remove();
        character_ = nullptr;
    }
}

bool CharacterHandler::HandleWorldData( Connection* connection, MemoryBuffer& message ) //@MSGID_WorldData
{
	//Set Character Node ID
	characterNodeID_ = message.ReadInt();
    mapIDToLoad_ = (MapID)message.ReadInt();

	//Change to World Screen
	SCREENMANAGER->ChangeScreen(ScreenType::World);

    return true;
}

void CharacterHandler::LoadCharacter()
{
    auto characterNode = character_->GetNode();

    if( characterNode )
    {
        //Load Animation Set
        character_->animationMgr_->Load( "Definitions/Animations/fighter.json" );

        //Set Camera Position
        CAMERAMANAGER->SetCameraType( CameraType::Follow, characterNode );
    }
}

void CharacterHandler::UnLoad()
{
    characterNodeID_ = -1;
    character_ = nullptr;
    selectedNode_ = nullptr;
    hoveredNode_ = nullptr;
    mapIDToLoad_ = MapID::Undefined;

    CAMERAMANAGER->SetCameraType( CameraType::Undefined, nullptr );
}

void CharacterHandler::ChangeAnimation( const Core::AnimationType& animationType, bool exclusive )
{
    if( character_ )
    {
        auto animationData = character_->animationMgr_->GetAnimationData( animationType );

        if( animationData )
            ChangeAnimation( animationData->id, exclusive );
    }
}

void CharacterHandler::HandleUpdate( StringHash eventType, VariantMap& eventData )
{
    using namespace CharacterData;

    if( SCREEN_TYPE == ScreenType::World && character_ && CONNECTIONG )
    {
        auto characterNode = character_->GetNode();

        if( characterNode )
        {
            //Set Connection Controls
            Controls controls;
            controls.yaw_ = CAMERAMANAGER->GetCameraYaw() + CAMERAMANAGER->GetMouseYaw();
            controls.extraData_[P_ANIMATIONID] = -1;
            controls.extraData_[P_MAPID] = (MAP_ID)MAPMANAGER->GetCurrentMapID();
            
            if( !USERINTERFACE->GetFocusElement() )
                controls.Set( CHARACTERCONTROL_Forward, INPUT->GetMouseButtonDown( MOUSEB_LEFT ) || isWalking_ );
            else
                isWalking_ = false;

            if( INPUT->GetMouseButtonDown( MOUSEB_RIGHT ) )
            {
                ChangeAnimation( Core::AnimationType::Attack );
            }
            
            //Have animation to set?
            if( animationToSet_.first_ != 0 )
            {
                controls.extraData_[P_ANIMATIONID] = animationToSet_.first_;
                controls.extraData_[P_ANIMATIONEXCLUSIVE] = animationToSet_.second_;
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
        if( character_ == nullptr )
        {
            if( ACTIVESCREEN == nullptr )
                return;

            auto characterNode = ACTIVESCREEN->GetScene()->GetNode( characterNodeID_ );

            //Character Node Found!
            if( characterNode )
            {
                characterNode->RemoveComponent<RigidBody>();
                characterNode->RemoveComponent<CollisionShape>();
                character_ = characterNode->GetComponent<Core::Character>( true );
                character_->CreatePhysicsComponent( 0.56f, 1.51f );
                character_->connection_ = CONNECTIONG;
                character_->animationMgr_ = characterNode->GetComponent<Core::AnimationEntity>( true );

                //Intercept Network Position
                characterNode->SetInterceptNetworkUpdate( "Network Position", true );
                characterNode->SetInterceptNetworkUpdate( "Network Rotation", true );
                
                //Disabled: characterNode->GetComponent<AnimationController>( true )->SetInterceptNetworkUpdate( "Network Animations", true );

                //Load Map
                MAPMANAGER->Load( mapIDToLoad_ );

                //Load Character
                LoadCharacter();
            }
        }

        //Find Hovered Node
        hoveredNode_ = CAMERAMANAGER->GetNodeRaycast();
	}
}

void CharacterHandler::HandleMouseDown( StringHash eventType, VariantMap& eventData )
{
    //Character isn't created yet
    if( character_ == nullptr )
        return;

    //Get Dynamic Navigation Mesh Pointer
    auto navigationMesh = character_->GetNode()->GetScene()->GetComponent<NavigationMesh>( true );

    //Has Dynamic Navigation Mesh?
    if( navigationMesh )
    {
        selectedNode_ = CAMERAMANAGER->GetNodeRaycast();

        //Self selecting? Ignore it!
        if( selectedNode_ == (character_ ? character_->GetNode() : nullptr) )
            selectedNode_ = nullptr;

        //Found Selected Node?
        if( selectedNode_ )
            character_->SetTargetPosition( selectedNode_->GetWorldPosition() );
        else
            character_->ResetTargetPosition();
    }

    if( eventData[MouseButtonDown::P_BUTTON].GetInt() == MOUSEB_LEFT && eventData[MouseButtonDown::P_CLICKS].GetInt() == 2 )
        isWalking_ = true;
    else
        isWalking_ = false;
}

void CharacterHandler::HandleNetworkUpdateSent( StringHash eventType, VariantMap & eventData )
{
    animationToSet_.first_ = 0;
}
