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

    if( auto user = USERMANAGER->GetUser( connection ); user )
    {
        VectorBuffer loadUserMsg;
        loadUserMsg.WriteString( connection->GetAddress() );
        loadUserMsg.WriteInt( connection->GetPort() );
        loadUserMsg.WriteString( user->GetAccountName() );
        loadUserMsg.WriteString( characterName );
        loadUserMsg.WriteInt( (int)characterClass );
        
        NETSERVER->Send( Net::ServerType::Master, Net::MSGID_LoadUser, true, true, loadUserMsg );
        NETSERVER->Send( Net::ServerType::Game, Net::MSGID_LoadUser, true, true, loadUserMsg );
    }
}