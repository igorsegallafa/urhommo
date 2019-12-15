#include "PrecompiledHeader.h"
#include "CharacterHandler.h"

CharacterHandler::CharacterHandler( Context* context ) :
    HandlerImpl( context )
{
}

CharacterHandler::~CharacterHandler()
{
}

bool CharacterHandler::HandleWorldData( Connection* connection, MemoryBuffer& message )
{
    return true;
}
