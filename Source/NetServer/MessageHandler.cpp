#include "PrecompiledHeader.h"
#include "MessageHandler.h"

bool MessageImpl::IsValid( Connection* connection )
{
    //Validate message before to process it
    for( const auto& validation : validations )
        if( !validation( connection ) )
            return false;

    return true;
}

bool MessageImpl::CanProcess( Connection* connection, MemoryBuffer& message )
{
    //Process message
    for( const auto& process : processing )
        if( !process( connection, message ) )
            return false;

    return true;
}

namespace Handler
{
Message::Message( Context* context_ ) : Object( context_ )
{
}

Message::~Message()
{
}

MessageImpl& Message::Handle( int messageID )
{
    //Message already exists, so we have to return the existing handler
    if( handlers.Find( messageID ) != handlers.End() )
        return *handlers[messageID];

    //Create a new message handler
    SharedPtr<MessageImpl> newHandler( new MessageImpl( context_ ) );
    handlers[messageID] = newHandler;

    //Return message handler pointer
    return *newHandler;
}

bool Message::HandleMessage( StringHash eventType, VariantMap& eventData )
{
    using namespace NetworkMessage;
    auto sender = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());

    //Check if message has any handler
    if( auto it = handlers.Find( eventData[P_MESSAGEID].GetInt() ); it != handlers.End() )
    {
        auto message = it->second_;

        //Validate Message with Global Validations before
        for( const auto& handler : validations )
            if( !handler( sender ) )
                return false;

        //Validate Message Authorization
        if( !message->IsValid( sender ) )
            return false;

        //Casting Message Data to Memory Buffer
        const PODVector<unsigned char>& data = eventData[P_DATA].GetBuffer();
        MemoryBuffer buffer( data );

        //Process Message with Global Processing Handlers before
        for( const auto& handler : processing )
            if( !handler( sender, buffer ) )
                return false;

        //Process Message
        if( !message->CanProcess( sender, buffer ) )
            return false;

        return true;
    }

    return false;
}
}