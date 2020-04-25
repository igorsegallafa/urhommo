#pragma once

#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>

class MessageImpl
{
public:
    //! Constructor.
    MessageImpl() : validations_{}, processing_{} {}

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
            validations_.Push( i );

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
            processing_.Push( i );

        return *this;

    }
private:
    Vector<std::function<bool( Connection* connection )>> validations_;  //!< List of Validation Handler.
    Vector<std::function<bool( Connection* connection, MemoryBuffer& message )>> processing_;    //!< List of Processing Handler.
};

namespace Handler
{
class Message
{
public:
    //! Constructor.
    Message();

    //! Deconstructor.
    ~Message();

    //! Add Global Validation Handler.
    void AddValidation( std::function<bool( int messageID, Connection* connection )> handler ){ validations_.Push( handler ); }

    //! Add Global Processing Handler.
    void AddProcessing( std::function<bool( int messageID, Connection* connection, MemoryBuffer& message )> handler ){ processing_.Push( handler ); }

    //! Register a Handler for some message ID. 
    MessageImpl& Handle( int messageID );

    //! Handle Message from Network.
    void HandleMessage( StringHash eventType, VariantMap& eventData );
private:
    HashMap<int, MessageImpl*> handlers;    //!< List of Messages Handlers.
    Vector<std::function<bool( int messageID, Connection* connection )>> validations_;  //!< List of Global Validation Handler.
    Vector<std::function<bool( int messageID, Connection* connection, MemoryBuffer& message )>> processing_;    //!< List of Global Processing Handler.
};
}