#pragma once

#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>

class MessageImpl : public Object
{
    URHO3D_OBJECT( MessageImpl, Object );
public:
    //! Constructor.
    MessageImpl( Context* context ) : Object( context ), validations{}, processing{} {}

    //! Deconstructor.
    ~MessageImpl() = default;

    //! Validate Message before proceed.
    bool IsValid( Connection* connection );

    //! Process Message.
    bool CanProcess( Connection* connection, MemoryBuffer& message );

    /**
     * Add a Validation Handler for this Message
     * @param handler List of ordered handlers used for validation
     * @return Self object reference
     */
    template<typename ...T>
    MessageImpl& Validate( T... handler )
    {
        std::function<bool( Connection * connection )>* args[]{ handler... };

        for( auto& i : args )
            validations.Push( i );

        return *this;
    }

    /**
     * Add a Processing Handler for this Message
     * @param handler List of ordered handlers used for Processing
     * @return Self object reference
     */
    template<typename ...T>
    MessageImpl& Process( T... handler )
    {
        std::function<bool( Connection* connection, MemoryBuffer& message )> args[]{ handler... };

        for( auto& i : args )
            processing.Push( i );

        return *this;

    }
private:
    Vector<std::function<bool( Connection* connection )>> validations;  //!< List of Validation Handler.
    Vector<std::function<bool( Connection* connection, MemoryBuffer& message )>> processing;    //!< List of Processing Handler.
};

namespace Handler
{
class Message : public Object
{
    URHO3D_OBJECT( Message, Object );
public:
    //! Constructor.
    Message( Context* context );

    //! Deconstructor.
    ~Message();

    //! Add Global Validation Handler.
    void AddValidation( std::function<bool( int messageID, Connection* connection )> handler ){ validations.Push( handler ); }

    //! Add Global Processing Handler.
    void AddProcessing( std::function<bool( int messageID, Connection* connection, MemoryBuffer& message )> handler ){ processing.Push( handler ); }

    //! Register a Handler for some message ID. 
    MessageImpl& Handle( int messageID );

    //! Handle Message from Network.
    void HandleMessage( StringHash eventType, VariantMap& eventData );
private:
    HashMap<int, MessageImpl*> handlers;    //!< List of Messages Handlers.
    Vector<std::function<bool( int messageID, Connection* connection )>> validations;  //!< List of Global Validation Handler.
    Vector<std::function<bool( int messageID, Connection* connection, MemoryBuffer& message )>> processing;    //!< List of Global Processing Handler.
};
}