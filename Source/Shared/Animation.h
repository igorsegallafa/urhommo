#pragma once

namespace Core
{
enum class AnimationType
{
    Undefined,
    Idle,
    Walk,
    Run,
    Attack,
};

enum AnimationLayer
{
    ANIMATIONLAYER_Default,
    ANIMATIONLAYER_Action,
    ANIMATIONLAYER_Exclusive,
};

struct AnimationData
{
    int id;
    AnimationType type;
    String file;
    bool loop;
};

const char* AnimationTypeToString( const AnimationType& animationType );
AnimationType AnimationTypeFromString( const String& str );

class AnimationEntity : public LogicComponent
{
    URHO3D_OBJECT( AnimationEntity, LogicComponent );
public:
    //! Default Constructor.
    AnimationEntity( Context* context );

    //! Deconstructor.
    ~AnimationEntity();

    //! Fixed Update Handler.
    void FixedUpdate( float time ) override;

    //! Load Animation Set.
    bool Load( const String& fileName );

    //! Play Animation.
    void Play( const AnimationType& animationType, bool exclusive = false );

    //! Play by Animation ID.
    void Play( int animationID, bool exclusive = false );

    //! Add Animation Data.
    void AddAnimationData( AnimationData animationData );

    //! Get a Random Animation Data.
    AnimationData* GetAnimationData( const AnimationType& animationType );

    //! Get current Animation Data.
    AnimationData* GetCurrentAnimationData();
private:
    Vector<AnimationData> animations;   //!< Animation Set.
};
};