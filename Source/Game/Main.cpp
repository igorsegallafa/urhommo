#include "PrecompiledHeader.h"
#include "Main.h"
#include "GameHandler.h"

Game::Game( Context* context ) : Application( context )
{
    GameHandler::RegisterLibrary( context );
    GameManager::RegisterLibrary( context );
    Shared::SetIsGameRunning( true );
    Shared::RegisterLibrary( context );

    SubscribeToEvent( E_POSTUPDATE, URHO3D_HANDLER( Game, HandlePostUpdate ) );
    SubscribeToEvent( E_CUSTOMGUIRENDERING, URHO3D_HANDLER( Game, HandleRender ) );
    SubscribeToEvent( E_SCREENMODE, URHO3D_HANDLER( Game, HandleWindowResized ) );

    //Input Events
    SubscribeToEvent( E_MOUSEBUTTONDOWN, URHO3D_HANDLER( Game, HandleMouseButtonDown ) );
    SubscribeToEvent( E_MOUSEBUTTONUP, URHO3D_HANDLER( Game, HandleMouseButtonUp ) );
    SubscribeToEvent( E_MOUSEMOVE, URHO3D_HANDLER( Game, HandleMouseMove ) );
    SubscribeToEvent( E_MOUSEWHEEL, URHO3D_HANDLER( Game, HandleMouseMove ) );
    SubscribeToEvent( E_KEYDOWN, URHO3D_HANDLER( Game, HandleKeyDown ) );
    SubscribeToEvent( E_KEYUP, URHO3D_HANDLER( Game, HandleKeyUp ) );
    SubscribeToEvent( E_TEXTINPUT, URHO3D_HANDLER( Game, HandleKeyChar ) );
    SubscribeToEvent( E_IMGUI_NEWFRAME, URHO3D_HANDLER( Game, HandleImGuiNewFrame ) );
}

Game::~Game()
{
}

void Game::Setup()
{
    engineParameters_[EP_WINDOW_TITLE] = "Game";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_HEIGHT] = 600;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
    engineParameters_[EP_WINDOW_RESIZABLE] = true;
    engineParameters_[EP_RESOURCE_PATHS] = "Data;";
}

void Game::Start()
{
    //Create ImGui
    imGui = MakeShared<imgui>( context_ );

    //Set Mouse Visible
    GetSubsystem<Input>()->SetMouseVisible( true );

    //Initialize Game Interface
    GAMEMANAGER->Init();
    GAMEHANDLER->Init();
}

void Game::Stop()
{
    GAMEHANDLER->UnInit();
    GAMEMANAGER->UnInit();
}

void Game::HandlePostUpdate( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleRender( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleWindowResized( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleMouseButtonDown( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleMouseButtonUp( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleMouseMove( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleMouseWheel( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleKeyDown( StringHash eventType, VariantMap& eventData )
{
    using namespace KeyDown;

    if( eventData[P_KEY] == KEY_ESCAPE && (INPUT->GetKeyDown( KEY_LSHIFT ) || INPUT->GetKeyDown( KEY_RSHIFT )) )
        engine_->Exit();
}

void Game::HandleKeyUp( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleKeyChar( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleImGuiNewFrame( StringHash eventType, VariantMap& eventData )
{
    //Debug Window
    ImGui::Begin( "Debug" );
    {
        ImGui::Text( "Hi ^_^" );

        ImGui::BeginTabBar( "Menu" );

        //General Tab
        if( ImGui::BeginTabItem( "General" ) )
        {
            ImGui::EndTabItem();
        }

        //Character Tab
        if( ImGui::BeginTabItem( "Character" ) )
        {
            ImGui::EndTabItem();
        }

        //Scene Tab
        if( ImGui::BeginTabItem( "Scene" ) )
        {
            float distanceToHoveredNode = 0.f;

            if( CHARACTERHANDLER->GetHoveredNode() && CHARACTERHANDLER->GetCharacter()->GetNode() )
                distanceToHoveredNode = abs((CHARACTERHANDLER->GetCharacter()->GetNode()->GetPosition() - CHARACTERHANDLER->GetHoveredNode()->GetPosition()).LengthSquared());

            ImGui::Text( "Current Map ID: %d", MAPMANAGER->GetCurrentMapID() );
            ImGui::Text( "Next Map ID: %d", MAPMANAGER->GetNextMapID() );
            ImGui::Text( "Selected Node: %s", (CHARACTERHANDLER->GetSelectedNode() ? CHARACTERHANDLER->GetSelectedNode()->GetName().CString() : "None") );
            ImGui::Text( "Hovered Node: %s (Distance: %f)", (CHARACTERHANDLER->GetHoveredNode() ? CHARACTERHANDLER->GetHoveredNode()->GetName().CString() : "None"), distanceToHoveredNode );
            ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );

            ImGui::EndTabItem();
        }

        //Server Tab
        if( ImGui::BeginTabItem( "Server" ) )
        {
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}
