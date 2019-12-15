#pragma once

class CharacterHandler : public HandlerImpl
{
    URHO3D_OBJECT( CharacterHandler, HandlerImpl );
public:
    //! Constructor.
    CharacterHandler( Context* context );

    //! Deconstructor.
    ~CharacterHandler();

    //! World Data Handler.
    bool HandleWorldData( Connection* connection, MemoryBuffer& message );
};