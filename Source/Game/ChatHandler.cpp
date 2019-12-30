#include "PrecompiledHeader.h"
#include "ChatHandler.h"

ChatHandler::ChatHandler( Context* context ) :
    HandlerImpl( context ),
    chatWindow( nullptr )
{
}

ChatHandler::~ChatHandler()
{
}

bool ChatHandler::Init()
{
    //Load UI Style
    auto style = RESOURCECACHE->GetResource<XMLFile>( "UI/DefaultStyle.xml" );

    //Load Layout from XML
    chatWindow = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/ChatWindow.xml" ), style );

    return true;
}

void ChatHandler::Load()
{
    //Add Window for UI
    USERINTERFACE->GetRoot()->AddChild( chatWindow );
}
