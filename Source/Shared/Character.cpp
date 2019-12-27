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

        if( !GetSubsystem<Network>()->IsServerRunning() )
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

        auto p = node_->GetComponent<AnimationController>( true );

        Variant out;
        controls.extraData_.TryGetValue( "Action", out );
        if( auto action = out.GetInt(); action != 0 )
        {
            if( action == 10 )
            {
                if( p->IsPlaying( 1 ) && p->IsAtEnd( 1 ) )
                {
                    p->Play( "Models/ani/char/ws_skill_fclj_single_jian.ani", 1, false, 0.f );
                    p->SetTime( "Models/ani/char/ws_skill_fclj_single_jian.ani", 0.f );
                    p->SetSpeed( "Models/ani/char/ws_skill_fclj_single_jian.ani", 1.7f );
                    p->SetAutoFade( "Models/ani/char/ws_skill_fclj_single_jian.ani", 0.1f );
                }
                else if( !p->IsPlaying( 1 ) )
                {
                    p->Play( "Models/ani/char/ws_skill_fclj_single_jian.ani", 1, false, 0.15f );
                    p->SetSpeed( "Models/ani/char/ws_skill_fclj_single_jian.ani", 1.7f );
                    p->SetAutoFade( "Models/ani/char/ws_skill_fclj_single_jian.ani", 0.1f );
                }
            }
        }
        else
        {
            //Animation Controller
            if( controls.buttons_ & CHARACTERCONTROL_Forward )
            {
                p->PlayExclusive( "Models/ani/char/walk.ani", 0, true, 0.3f );
            }
            else
                p->PlayExclusive( "Models/ani/char/jxbws_stand_both_jian.ani", 0, true, 0.3f );
        }

    }
}
};