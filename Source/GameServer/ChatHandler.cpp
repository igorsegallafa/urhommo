#include "PrecompiledHeader.h"
#include "ChatHandler.h"

ChatHandler::ChatHandler( Context* context ) :
    HandlerImpl( context )
{
}

ChatHandler::~ChatHandler()
{
}

bool ChatHandler::HandleChatGame( Connection* connection, MemoryBuffer& message )
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

            }
            else
            {
                user->character->GetNode()->GetChild( "ChatNode" )->GetComponent<Text3D>()->SetText( messageChat );

                //Broadcast Message
                VectorBuffer chatGameMsg;
                chatGameMsg.WriteString( user->characterName + ": " + messageChat );
                NETWORK->BroadcastMessage( MSGID_ChatGame, true, true, chatGameMsg );
            }
        }

        return true;
    }

    return false;
}