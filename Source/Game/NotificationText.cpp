#include "PrecompiledHeader.h"
#include "NotificationText.h"

NotificationText::NotificationText( Context* context ) : Object( context )
{
    gui_ = USERINTERFACE->GetRoot()->CreateChild<UIElement>();
    gui_->SetSize( GRAPHICS->GetSize() );

    //Subscribe Events
    SubscribeToEvent( E_SCREENMODE, URHO3D_HANDLER( NotificationText, HandleScreenMode ) );
    SubscribeToEvent( E_UPDATE, URHO3D_HANDLER( NotificationText, HandleUpdate ) );
}

NotificationText::~NotificationText()
{
    gui_ = nullptr;
}

void NotificationText::Push( const String& message )
{
    if( notifications_.Size() )
    {
        if( notifications_.Back().text_->GetText().Compare( message ) == 0 )
            return;
    }

    int i = gui_->GetChildren().Size();
    for( auto& child : gui_->GetChildren() )
    {
        child->RemoveAttributeAnimation( "Position" );
        child->RemoveAttributeAnimation( "Opacity" );

        SharedPtr<ValueAnimation> updatePositionAnimation( new ValueAnimation( context_ ) );
        updatePositionAnimation->SetInterpolationMethod( IM_SPLINE );
        updatePositionAnimation->SetSplineTension( 0.3f );
        updatePositionAnimation->SetKeyFrame( 0.0f, IntVector2( 0, child->GetPosition().y_ ) );
        updatePositionAnimation->SetKeyFrame( 0.2f, IntVector2( 0, 150 + (30 * i) ) );
        child->SetAttributeAnimation( "Position", updatePositionAnimation, WrapMode::WM_ONCE );

        SharedPtr<ValueAnimation> opacityAnimation( new ValueAnimation( context_ ) );
        opacityAnimation->SetKeyFrame( 0.0f, child->GetOpacity() );
        opacityAnimation->SetKeyFrame( 0.2f, 1.f - (0.3f * i) );
        child->SetAttributeAnimation( "Opacity", opacityAnimation, WrapMode::WM_ONCE );

        i--;
    }

    NotificationTextData notification;
    notification.elapedTime_ = 0.f;
    notification.fadingOut_ = false;

    notification.text_ = gui_->CreateChild<Text>();
    notification.text_->SetFont( RESOURCECACHE->GetResource<Font>( "Fonts/SCHLBKB.ttf" ), 18 );
    notification.text_->SetHorizontalAlignment( HorizontalAlignment::HA_CENTER );
    notification.text_->SetText( message );
    notification.text_->SetTextEffect( TextEffect::TE_SHADOW );
    notification.text_->SetEffectShadowOffset( IntVector2( 0, 1 ) );
    notification.text_->SetColor( Color::YELLOW );
    notification.text_->SetOpacity( 1.0f );

    SharedPtr<ValueAnimation> positionAnimation( new ValueAnimation( context_ ) );
    positionAnimation->SetInterpolationMethod( IM_SPLINE );
    positionAnimation->SetSplineTension( 0.3f );
    positionAnimation->SetKeyFrame( 0.0f, IntVector2( 0, 0 ) );
    positionAnimation->SetKeyFrame( 0.1f, IntVector2( 0, 160 ) );
    positionAnimation->SetKeyFrame( 0.2f, IntVector2( 0, 150 ) );
    notification.text_->SetAttributeAnimation( "Position", positionAnimation, WrapMode::WM_ONCE );

    SharedPtr<ValueAnimation> opacityAnimation( new ValueAnimation( context_ ) );
    opacityAnimation->SetKeyFrame( 0.0f, 0.f );
    opacityAnimation->SetKeyFrame( 0.2f, 1.f );
    notification.text_->SetAttributeAnimation( "Opacity", opacityAnimation, WrapMode::WM_ONCE );
    notifications_.Push( notification );
  
    gui_->SetPriority( M_MAX_INT );
    gui_->SetBringToBack( false );
    gui_->SetBringToFront( true );
    gui_->BringToFront();
}

void NotificationText::Clear()
{
    for( auto& notification : notifications_ )
        notification.elapedTime_ = 2.f;
}

void NotificationText::Reset()
{
    for( auto& notification : notifications_ )
        notification.text_->Remove();

    notifications_.Clear();
}

void NotificationText::RegisterObject( Context* context )
{
    context->RegisterSubsystem<NotificationText>();
}

void NotificationText::HandleUpdate( StringHash eventType, VariantMap& eventData )
{
    using namespace Update;

    for( auto it = notifications_.Begin(); it != notifications_.End(); )
    {
        auto& notification = *it;
        notification.elapedTime_ += eventData[P_TIMESTEP].GetFloat();

        if( notification.elapedTime_ > 3.0f )
        {
            notification.text_->Remove();
            it = notifications_.Erase( it );
            continue;
        }
        else if( notification.elapedTime_ > 2.8f && notification.fadingOut_ == false )
        {
            notification.fadingOut_ = true;
            notification.text_->RemoveAttributeAnimation( "Opacity" );

            SharedPtr<ValueAnimation> opacityAnimation( new ValueAnimation( context_ ) );
            opacityAnimation->SetKeyFrame( 0.0f, notification.text_->GetOpacity() );
            opacityAnimation->SetKeyFrame( 0.2f, 0.f );
            notification.text_->SetAttributeAnimation( "Opacity", opacityAnimation, WrapMode::WM_ONCE );
        }

        ++it;
    }
}

void NotificationText::HandleScreenMode( StringHash eventType, VariantMap& eventData )
{
    if( gui_ )
        gui_->SetSize( GRAPHICS->GetSize() );
}
