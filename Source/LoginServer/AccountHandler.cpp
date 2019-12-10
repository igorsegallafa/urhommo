#include "PrecompiledHeader.h"
#include "AccountHandler.h"

AccountHandler::AccountHandler( Context* context ) :
    HandlerImpl( context )
{
}

AccountHandler::~AccountHandler()
{
}

bool AccountHandler::HandleCreateCharacter( Connection* connection, MemoryBuffer& message )
{
    auto characterName = message.ReadString();
    auto characterClass = (Core::CharacterClass)message.ReadInt();

    //TODO: Create Character into Database

    //Process World Data for Client
    ProcessWorldData( connection, characterName, characterClass, 1, Vector3::ZERO );

    return true;
}

void AccountHandler::ProcessWorldData( Connection* connection, const String& characterName, const Core::CharacterClass& characterClass,
                                       int characterLevel, const Vector3& position )
{
    //TODO: Input Validation

    VectorBuffer worldDataMsg;
    worldDataMsg.WriteString( characterName );
    worldDataMsg.WriteInt( (int)characterClass );
    worldDataMsg.WriteInt( characterLevel );
    worldDataMsg.WriteVector3( position );

    //Send Message
    connection->Send( MSGID_WorldData, true, true, worldDataMsg );
}
