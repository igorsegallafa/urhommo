#include "PrecompiledHeader.h"
#include "Animation.h"

namespace Core
{
AnimationEntity::AnimationEntity( Context* context ) :
    LogicComponent( context )
{
}

AnimationEntity::~AnimationEntity()
{
}

void AnimationEntity::FixedUpdate( float time )
{
}

bool AnimationEntity::Load( const String& fileName )
{
    auto file = GetSubsystem<ResourceCache>()->GetResource<JSONFile>( fileName );

    if( file )
    {
        auto root = file->GetRoot();

        for( const auto& anim : root.GetArray() )
        {
            AnimationData animationData;
            animationData.type = AnimationTypeFromString( anim["type"].GetString() );
            animationData.file = anim["file"].GetString();
            animationData.loop = anim["loop"].GetBool();
            AddAnimationData( animationData );
        }

        return true;
    }

    return false;
}

void AnimationEntity::Play( const AnimationType& animationType )
{
    auto animationData = GetAnimationData( animationType );

    if( animationData )
        Play( animationData->id );
}

void AnimationEntity::Play( int animationID )
{
    //Invalid Animation
    if( animationID < 0 || animationID >= animations.Size() )
        return;

    auto animationData = &animations[animationID];
    auto animationController = node_->GetComponent<AnimationController>( true );

    if( animationData )
    {
        if( animationData->type > AnimationType::Run )
        {
            //Keep the same animation? So we don't need to stop the layer
            if( animationController->IsPlaying( ANIMATIONLAYER_Action ) && animationController->IsAtEnd( ANIMATIONLAYER_Action ) )
            {
                animationController->Play( animationData->file, ANIMATIONLAYER_Action, animationData->loop, 0.15f );
                animationController->SetAutoFade( animationData->file, 0.1f );
            }
            else if( !animationController->IsPlaying( ANIMATIONLAYER_Action ) )
            {
                animationController->Play( animationData->file, ANIMATIONLAYER_Action, animationData->loop, 0.15f );
                animationController->SetAutoFade( animationData->file, 0.1f );
            }
        }
        else
        {
            animationController->PlayExclusive( animationData->file, ANIMATIONLAYER_Default, animationData->loop, 0.3f );
        }
    }
}

void AnimationEntity::AddAnimationData( AnimationData animationData )
{
    //Update Animation Data ID
    animationData.id = animations.Size();

    //Push it to Vector
    animations.Push( animationData );
}

AnimationData* AnimationEntity::GetAnimationData( const AnimationType& animationType )
{
    Vector<AnimationData*> animationsFound;

    for( auto& animationData : animations )
        if( animationData.type == animationType )
            animationsFound.Push( &animationData );

    if( animationsFound.Size() )
        return animationsFound[Random( 0, animationsFound.Size())];

    return nullptr;
}

AnimationData * AnimationEntity::GetCurrentAnimationData()
{
    auto animationController = node_->GetComponent<AnimationController>( true );

    for( auto& animationData : animations )
        if( animationController->IsPlaying( animationData.file ) )
        {
            if( animationController->IsPlaying( ANIMATIONLAYER_Action ) && animationData.type <= AnimationType::Run )
                continue;

            return &animationData;
        }

    return nullptr;
}

const char* AnimationTypeToString( const AnimationType& animationType )
{
    switch( animationType )
    {
        case AnimationType::Idle:
            return "idle";
        case AnimationType::Walk:
            return "walk";
        case AnimationType::Run:
            return "run";
        case AnimationType::Attack:
            return "attack";
    }

    return "undefined";
}

AnimationType AnimationTypeFromString( const String& str )
{
    if( str.Compare( "idle", false ) == 0 )
        return AnimationType::Idle;
    else if( str.Compare( "walk", false ) == 0 )
        return AnimationType::Walk;
    else if( str.Compare( "run", false ) == 0 )
        return AnimationType::Run;
    else if( str.Compare( "attack", false ) == 0 )
        return AnimationType::Attack;

    return AnimationType::Undefined;
}
};