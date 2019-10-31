#include "PrecompiledHeader.h"
#include "Main.h"

Main::Main( Context* context ) : Application( context )
{
}

void Main::Setup()
{
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_HEIGHT] = 600;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
}

void Main::Start()
{
    GetSubsystem<Input>()->SetMouseVisible( true );
}

void Main::Stop()
{
}