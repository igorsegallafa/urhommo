#include "PrecompiledHeader.h"
#include "Character.h"

#undef new

#include <Bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>

namespace Core
{
Character::Character( Context* context ) :
    Entity( context ),
    connection( nullptr )
{
    height = 1.51f;
    diameter = 0.56f;
}

Character::~Character()
{
}

void Character::FixedUpdate( float time )
{
    if( connection )
    {
        const Controls& controls = connection->GetControls();

        if( Shared::IsGameRunning() )
        {
            Quaternion rotation;
            Vector3 moveDirection = Vector3::ZERO;

            auto transform = GetComponent<SmoothedTransform>();

            //Smoothed Transform not found
            if( transform == nullptr || bulletController == nullptr )
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
            if( followingTarget )
            {
                moveDirection = targetDirection;
                rotation.FromLookRotation( targetDirection );
                transform->SetTargetRotation( rotation );
            }

            //Move Direction is valid?
            if( moveDirection.LengthSquared() > 0.0f )
            {
                //It's in a ground?
                if( bulletController->onGround() )
                    bulletController->setWalkDirection( ToBtVector3( moveDirection * time * 5.f ) );
                else
                    bulletController->setWalkDirection( ToBtVector3( moveDirection * time * 5.f ) );
            }
            else
                bulletController->setWalkDirection( btVector3( 0, 0, 0 ) );

            //Get World Transform
            btTransform worldTransform;
            worldTransform = bulletController->getGhostObject()->getWorldTransform();

            //Set World Position
            Vector3 newPosition = ToVector3( worldTransform.getOrigin() ) + Vector3::DOWN * height * 0.5f;
            node_->SetWorldPosition( newPosition );
        }
        else if( connection->GetPosition() != Vector3( -1.f, -1.f, -1.f ) )
        {
            node_->SetRotation( connection->GetRotation() );
            node_->SetPosition( connection->GetPosition() );
        }

        //Get Animation Current ID
        Variant animationOut;
        controls.extraData_.TryGetValue( "AnimationID", animationOut );

        //Set Character Animation
        if( auto animationID = animationOut.GetInt(); animationID != -1 )
            animationMgr->Play( animationID ); 
        else if( controls.buttons_ & CHARACTERCONTROL_Forward )
            animationMgr->Play( AnimationType::Walk );
        else
            animationMgr->Play( AnimationType::Idle );

    }
}
};