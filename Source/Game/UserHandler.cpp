#include "PrecompiledHeader.h"
#include "UserHandler.h"

UserHandler::UserHandler( Context* context ) :
    HandlerImpl( context )
{
}

UserHandler::~UserHandler()
{
}

void UserHandler::SelectCharacter( const String& characterName )
{
    if( SCREEN_TYPE == ScreenType::Character )
    {
        VectorBuffer selectCharacterMsg;
        selectCharacterMsg.WriteString( characterName );
        CONNECTIONL->Send( MSGID_SelectCharacter, true, true, selectCharacterMsg );
    }
}

void UserHandler::CreateCharacter( const String& characterName, const CharacterClass& characterClass )
{
    if( SCREEN_TYPE == ScreenType::Character )
    {
        VectorBuffer createCharacterMsg;
        createCharacterMsg.WriteString( characterName );
        createCharacterMsg.WriteInt( (int)characterClass );
        CONNECTIONL->Send( MSGID_CreateCharacter, true, true, createCharacterMsg );
    }
}