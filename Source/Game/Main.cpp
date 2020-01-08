#include "PrecompiledHeader.h"
#include "Main.h"
#include "GameHandler.h"

void LogHandler( const char* filename, uint32_t line, uint32_t level, const char* channel, const char* message )
{
    if( strcmp( channel, "" ) == 0 )
    {
        if( level == 0 )
            URHO3D_LOGTRACEF( "%s", message );
        else if( level == 1 )
            URHO3D_LOGDEBUGF( "%s", message );
        else if( level == 2 )
            URHO3D_LOGINFOF( "%s", message );
        else if( level == 3 )
            URHO3D_LOGWARNINGF( "%s", message );
        else if( level == 4 )
            URHO3D_LOGERRORF( "%s", message );
    }
}

Game::Game( Context* context ) : Application( context ),
    view( nullptr )
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
    Noesis::GUI::Init( nullptr, LogHandler, nullptr );

    //For simplicity purposes we are not using resource providers in this sample. ParseXaml() is
    //enough if there is no extra XAML dependencies
    Noesis::Ptr<Noesis::Grid> xaml( Noesis::GUI::ParseXaml<Noesis::Grid>( R"(
        <Grid xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
    <Viewbox>
        <StackPanel Margin="50">
            <Button Content="vsf" Margin="0,30,0,0" Width="83.596"/>
            <TextBox Height="16" TextWrapping="Wrap" Text="TextBox" Width="48.668"/>
            <Rectangle Height="5" Margin="-10,20,-10,0">
                <Rectangle.Fill>
                    <RadialGradientBrush>
                        <GradientStop Offset="0" Color="#40000000"/>
                        <GradientStop Offset="1" Color="#00000000"/>
                    </RadialGradientBrush>
                </Rectangle.Fill>
            </Rectangle>
        </StackPanel>
    </Viewbox>
</Grid>
    )" ) );

    //Create NoesisGUI View from XAML
    view = Noesis::GUI::CreateView( xaml ).GiveOwnership();
    view->SetIsPPAAEnabled( true );
    view->SetSize( GRAPHICS->GetWidth(), GRAPHICS->GetHeight() );

    //Initialize NoesisGUI Renderer
    view->GetRenderer()->Init( NoesisApp::GLFactory::CreateDevice() );

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
    //Update view (layout, animations, ...) of NoesisGUI
    view->Update( eventData[PostUpdate::P_TIMESTEP].GetFloat() );
}

void Game::HandleRender( StringHash eventType, VariantMap& eventData )
{
    //Offscreen rendering phase populates textures needed by the on-screen rendering
    view->GetRenderer()->UpdateRenderTree();
    view->GetRenderer()->RenderOffscreen();

    //Render it
    view->GetRenderer()->Render();
}

void Game::HandleWindowResized( StringHash eventType, VariantMap& eventData )
{
    if( view )
        view->SetSize( eventData[ScreenMode::P_WIDTH].GetInt(), eventData[ScreenMode::P_HEIGHT].GetInt() );
}

void Game::HandleMouseButtonDown( StringHash eventType, VariantMap& eventData )
{
    if( eventData[MouseButtonDown::P_CLICKS].GetInt() == 2 )
        view->MouseDoubleClick( INPUT->GetMousePosition().x_, INPUT->GetMousePosition().y_, eventData[MouseButtonDown::P_BUTTON].GetInt() == MOUSEB_RIGHT ? Noesis::MouseButton_Right : Noesis::MouseButton_Left );
    else
        view->MouseButtonDown( INPUT->GetMousePosition().x_, INPUT->GetMousePosition().y_, eventData[MouseButtonDown::P_BUTTON].GetInt() == MOUSEB_RIGHT ? Noesis::MouseButton_Right : Noesis::MouseButton_Left );
}

void Game::HandleMouseButtonUp( StringHash eventType, VariantMap& eventData )
{
    view->MouseButtonUp( INPUT->GetMousePosition().x_, INPUT->GetMousePosition().y_, eventData[MouseButtonDown::P_BUTTON].GetInt() == MOUSEB_RIGHT ? Noesis::MouseButton_Right : Noesis::MouseButton_Left );
}

void Game::HandleMouseMove( StringHash eventType, VariantMap& eventData )
{
    view->MouseMove( eventData[MouseMove::P_X].GetInt(), eventData[MouseMove::P_Y].GetInt() );
}

void Game::HandleMouseWheel( StringHash eventType, VariantMap& eventData )
{
    view->MouseWheel( INPUT->GetMousePosition().x_, INPUT->GetMousePosition().y_, eventData[MouseWheel::P_WHEEL].GetInt() );
}

void Game::HandleKeyDown( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleKeyUp( StringHash eventType, VariantMap& eventData )
{
}

void Game::HandleKeyChar( StringHash eventType, VariantMap& eventData )
{
    view->Char( eventData[TextInput::P_TEXT].GetString()[0] );
}
