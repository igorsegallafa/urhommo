#include "PrecompiledHeader.h"
#include "AccountHandler.h"

AccountHandler::AccountHandler( Context* context ) :
    HandlerImpl( context )
{
}

AccountHandler::~AccountHandler()
{
}

void AccountHandler::CreateCharacter( const String& characterName, const Core::CharacterClass& characterClass )
{
    if( SCREEN_TYPE == ScreenType::Character )
    {
        VectorBuffer createCharacterMsg;
        createCharacterMsg.WriteString( characterName );
        createCharacterMsg.WriteInt( (int)characterClass );
        CONNECTIONL->Send( MSGID_CreateCharacter, true, true, createCharacterMsg );
    }
}

bool AccountHandler::HandleWorldData( Connection* connection, MemoryBuffer& message )
{
    //We are already on Master and Game Server, we don't need Login Server anymore
    NETWORKHANDLER->CloseLoginServer();

    //TODO: Process Message Data

    //Change Screen to World Server
    SCREENMANAGER->ChangeScreen( ScreenType::World );

    return true;
}