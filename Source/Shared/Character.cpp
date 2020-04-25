#include "PrecompiledHeader.h"
#include "Character.h"

#undef new

#include "CharacterDef.h"

#include <Bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>

namespace Core
{
Character::Character( Context* context ) :
    Entity( context ),
    connection_( nullptr ),
    equipmentMgr_( nullptr )
{
}

Character::~Character()
{
    connection_ = nullptr;
    equipmentMgr_ = nullptr;
}

void Character::FixedUpdate( float time )
{
    using namespace CharacterData;

    if( connection_ )
    {
        const Controls& controls = connection_->GetControls();

        if( Shared::IsGameRunning() )
        {
            Quaternion rotation;
            Vector3 moveDirection = Vector3::ZERO;

            auto transform = GetComponent<SmoothedTransform>();

            //Smoothed Transform not found
            if( transform == nullptr || bulletController_ == nullptr )
                return;

            //Character is walking?
            if( controls.buttons_ & CHARACTERCONTROL_Forward )
            {
                moveDirection += Vector3::FORWARD;
                moveDirection = node_->GetWorldRotation() * moveDirection;
                rotation = Quaternion( 0.0f, controls.yaw_, 0.0f );
                transform->SetTargetRotation( rotation );
            }

            //Following Target
            if( followingTarget_ )
            {
                moveDirection = targetDirection_;
                rotation.FromLookRotation( targetDirection_ );
                transform->SetTargetRotation( rotation );
            }

            //Move Direction is valid?
            if( moveDirection.LengthSquared() > 0.0f )
            {
                //It's in a ground?
                if( bulletController_->onGround() )
                    bulletController_->setWalkDirection( ToBtVector3( moveDirection * time * 5.f ) );
                else
                    bulletController_->setWalkDirection( ToBtVector3( moveDirection * time * 5.f ) );
            }
            else
                bulletController_->setWalkDirection( btVector3( 0, 0, 0 ) );

            //Get World Transform
            btTransform worldTransform;
            worldTransform = bulletController_->getGhostObject()->getWorldTransform();

            //Set World Position
            Vector3 newPosition = ToVector3( worldTransform.getOrigin() ) + Vector3::DOWN * height_ * 0.5f;
            node_->SetWorldPosition( newPosition );
        }
        else if( connection_->GetPosition() != Vector3( -1.f, -1.f, -1.f ) )
        {
            node_->SetRotation( connection_->GetRotation() );
            node_->SetPosition( connection_->GetPosition() );
        }

        //Get Animation Current ID
        Variant animationOut;

        //Set Character Animation
        if( controls.extraData_.TryGetValue( P_ANIMATIONID, animationOut ) )
            if( auto animationID = animationOut.GetInt(); animationID != -1 )
                animationMgr_->Play( animationID, controls.extraData_[P_ANIMATIONEXCLUSIVE]->GetBool() );
        else if( controls.buttons_ & CHARACTERCONTROL_Forward || followingTarget_ )
            animationMgr_->Play( AnimationType::Run );
        else
            animationMgr_->Play( AnimationType::Idle );
    }

    Entity::FixedUpdate( time );
}
};