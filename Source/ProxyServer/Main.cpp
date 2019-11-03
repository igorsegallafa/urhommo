#include "PrecompiledHeader.h"
#include "Main.h"

Main::Main( Context* context ) : Application( context )
{
    Net::Manager::Server::RegisterServerLibrary( context );
}

void Main::Setup()
{
    engineParameters_[EP_WINDOW_TITLE] = "Proxy Server";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_HEADLESS] = true;
    engineParameters_[EP_SOUND] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = false;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
    engineParameters_[EP_WINDOW_HEIGHT] = 416;
}

void Main::Start()
{
    OpenConsoleWindow();
}

void Main::Stop()
{
}