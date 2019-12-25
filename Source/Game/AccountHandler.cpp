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