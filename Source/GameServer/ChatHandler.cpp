#include "PrecompiledHeader.h"
#include "ChatHandler.h"

ChatHandler::ChatHandler( Context* context ) :
    HandlerImpl( context )
{
}

ChatHandler::~ChatHandler()
{
}

bool ChatHandler::HandleChatGame( Connection* connection, MemoryBuffer& message ) //@MSGID_ChatGame
{
    auto user = USERMANAGER->GetUser( connection );

    if( user )
    {
        auto messageChat = message.ReadString();

        if( messageChat.Length() )
        {
            //Chat Command
            if( messageChat[0] == '/' )
            {
                auto chatCommand = messageChat.Substring( 0, messageChat.Find( ' ' ) );

                if( chatCommand.Compare( "/SetArmor", false ) == 0 )
                {
                    if( String param1; GetParameterString( messageChat, 1, param1 ) )
                    {
                        user->character_->equipmentMgr_->SetArmor( EQUIPMENTMANAGER->GetEquipModelData( EquipType::Armor, CharacterClass::Fighter, ToInt( param1 ) ) );
                    }
                }
                else if( chatCommand.Compare( "/SetHead", false ) == 0 )
                {
                    if( String param1; GetParameterString( messageChat, 1, param1 ) )
                    {
                        user->character_->equipmentMgr_->SetHead( EQUIPMENTMANAGER->GetEquipModelData( EquipType::Head, CharacterClass::Fighter, ToInt( param1 ) ) );
                    }
                }
            }
            else
            {
                //Broadcast Message
                VectorBuffer chatGameMsg;
                chatGameMsg.WriteString( user->characterName_ + ": " + messageChat );
                NETWORK->BroadcastMessage( MSGID_ChatGame, true, true, chatGameMsg );
            }
        }

        return true;
    }

    return false;
}