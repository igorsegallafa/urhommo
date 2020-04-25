#include "PrecompiledHeader.h"
#include "Screen.h"

void Screen::Init()
{
    //Create the Scene
    scene_ = MakeShared<Scene>( context_ );
    gui_ =  MakeShared<UIElement>( context_ );

    //Add GUI for UI Root
    gui_->SetSize( GRAPHICS->GetSize() );
    USERINTERFACE->GetRoot()->AddChild( gui_ );

    //Subscribe Events
    SubscribeToEvent( E_SCREENMODE, URHO3D_HANDLER( Screen, HandleScreenMode ) );
}

void Screen::HandleScreenMode( StringHash eventType, VariantMap& eventData )
{
    if( gui_ )
        gui_->SetSize( GRAPHICS->GetSize() );
}

void Screen::Run()
{
    scene_->SetUpdateEnabled( true );
}

void Screen::Pause()
{
    scene_->SetUpdateEnabled( false );
}

void Screen::Dispose()
{
    //Pause the scene and remove everything from it
    if( scene_ )
    {
        scene_->SetUpdateEnabled( false );
        scene_->Clear();
        scene_->Remove();
    }

    if( gui_ )
    {
        gui_->RemoveAllChildren();
        gui_->Remove();
    }
}