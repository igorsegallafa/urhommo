#include "PrecompiledHeader.h"
#include "LoginScreen.h"

LoginScreen::LoginScreen( Context* context ) : 
    Screen( context ),
    loginWindow( nullptr ),
    gameServerWindow( nullptr )
{
}

LoginScreen::~LoginScreen()
{
}

void LoginScreen::Init()
{
    Screen::Init();

    //Create Scene
    CreateScene();

    //Setup Viewport
    SetupViewport();

    //Build Window
    BuildWindow();
}

void LoginScreen::CreateScene()
{
    auto sceneFile = RESOURCECACHE->GetResource<XMLFile>( "Scenes/Login.xml" );

    if( sceneFile )
    {
        scene->LoadXML( sceneFile->GetRoot() );

        if( auto birdNode = scene->GetChild( "s_login_niao", true ); birdNode )
        {
            if( auto animCtrl = birdNode->GetComponent<AnimationController>(); animCtrl )
                animCtrl->PlayExclusive( "Models/login/s_login_niao_Take.ani", 0, true );
        }
    }
}

void LoginScreen::SetupViewport()
{
    //Create Viewport and Set it
    SharedPtr<Viewport> viewport( new Viewport( context_, scene, CAMERA ) );
    RENDERER->SetViewport( 0, viewport );
}

void LoginScreen::BuildWindow()
{
    //Load UI Style
    auto style = RESOURCECACHE->GetResource<XMLFile>( "UI/DefaultStyle.xml" );

    //Load Layout from XML
    loginWindow = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/Login.xml" ), style );
    gameServerWindow = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/LoginGameServer.xml" ), style );

    //Load Window
    loginWindow->SetVisible( true );
    gameServerWindow->SetVisible( false );

    //Subscribe Events
    SubscribeToEvent( loginWindow->GetChild( "ButtonLogin", true ), E_RELEASED, URHO3D_HANDLER( LoginScreen, HandleLoginButtonPressed ) );

    //Add Window for UI
    USERINTERFACE->GetRoot()->AddChild( loginWindow );
    USERINTERFACE->GetRoot()->AddChild( gameServerWindow );
}

void LoginScreen::SetGameServerList( const Vector<String>& gameServerList )
{
    if( gameServerWindow && loginWindow )
    {
        gameServerWindow->RemoveAllChildren();

        Text* pSelectServerText = new Text( context_ );
        pSelectServerText->SetText( "Selecione um Servidor" );
        pSelectServerText->SetFont( RESOURCECACHE->GetResource<Font>( "Fonts/Anonymous Pro.ttf" ), 12 );
        pSelectServerText->SetColor( Color::WHITE );
        pSelectServerText->SetPosition( 0, 0 );
        pSelectServerText->SetTextEffect( TextEffect::TE_STROKE );
        pSelectServerText->SetAlignment( HA_CENTER, VA_TOP );
        gameServerWindow->AddChild( pSelectServerText );

        int totalServers = 0;
        for( const auto pServerName : gameServerList )
        {
            Button* textButton = new Button( context_ );
            textButton->SetAlignment( HA_CENTER, VA_TOP );
            textButton->SetSize( 150, 20 );
            textButton->SetPosition( 0, 50 + (30 * totalServers) );
            textButton->SetStyleAuto();
            gameServerWindow->AddChild( textButton );
            SubscribeToEvent( textButton, E_PRESSED, std::bind( &LoginScreen::HandleGameServerPressed, this, totalServers ) );

            Text* serverText = new Text( context_ );
            serverText->SetText( pServerName );
            serverText->SetFont( RESOURCECACHE->GetResource<Font>( "Fonts/Anonymous Pro.ttf" ), 12 );
            serverText->SetColor( Color::WHITE );
            serverText->SetPosition( 0, 50 + (30 * totalServers) );
            serverText->SetTextEffect( TextEffect::TE_SHADOW );
            serverText->SetAlignment( HA_CENTER, VA_TOP );
            gameServerWindow->AddChild( serverText );

            totalServers++;
        }

        loginWindow->SetVisible( false );
        gameServerWindow->SetVisible( true );
    }
}

void LoginScreen::HandleGameServerPressed( int serverIndex )
{
    //We must to connect to all servers before to send/receive any message
    LOGINHANDLER->ProcessGameServer( serverIndex );

    //TODO: Connect Chat Server?
}

void LoginScreen::HandleLoginButtonPressed( StringHash eventType, VariantMap& eventData )
{
    auto lineEditAccount = loginWindow->GetChildStaticCast<LineEdit>( "Account", true );
    auto lineEditPassword = loginWindow->GetChildStaticCast<LineEdit>( "Password", true );

    LOGINHANDLER->ProcessLogin( lineEditAccount->GetText(), lineEditPassword->GetText() );
}