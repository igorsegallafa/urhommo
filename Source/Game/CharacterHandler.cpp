#include "PrecompiledHeader.h"
#include "CharacterHandler.h"

CharacterHandler::CharacterHandler( Context* context ) :
    HandlerImpl( context ),
	characterNodeID( -1 ),
    character( nullptr )
{
	SubscribeToEvent( E_POSTUPDATE, URHO3D_HANDLER( CharacterHandler, HandlePostUpdate ) );
}

CharacterHandler::~CharacterHandler()
{
    if( character )
    {
        character->Remove();
        character = nullptr;
    }
}

bool CharacterHandler::HandleWorldData( Connection* connection, MemoryBuffer& message )
{
	//Set Character Node ID
	characterNodeID = message.ReadInt();
    mapIDToLoad = (MapID)message.ReadInt();

	//Change to World Screen
	SCREENMANAGER->ChangeScreen(ScreenType::World);

    return true;
}

void CharacterHandler::LoadCharacter()
{
    auto characterNode = character->GetNode();

    if( characterNode )
    {
        //Set Camera Position
        CAMERAMANAGER->SetCameraType( CameraType::Follow, characterNode );
    }
}

void CharacterHandler::HandlePostUpdate( StringHash eventType, VariantMap& eventData )
{
	if( SCREEN_TYPE == ScreenType::World )
	{
        //Look for Character Component
        if( character == nullptr )
        {
            auto characterNode = ACTIVESCREEN->GetScene()->GetNode( characterNodeID );

            //Character Node Found!
            if( characterNode )
            {
                character = characterNode->GetComponent<Core::Character>( true );

                //Load Map
                MAPMANAGER->Load( mapIDToLoad );

                //Load Character
                LoadCharacter();
            }
        }
	}
}
