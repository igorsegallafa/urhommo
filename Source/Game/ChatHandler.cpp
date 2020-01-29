#include "PrecompiledHeader.h"
#include "ChatHandler.h"

ChatHandler::ChatHandler( Context* context ) :
    HandlerImpl( context ),
    chatWindow( nullptr )
{
}

ChatHandler::~ChatHandler()
{
}

bool ChatHandler::Init()
{
    //Load UI Style
    auto style = RESOURCECACHE->GetResource<XMLFile>( "UI/DefaultStyle.xml" );

    //Load Layout from XML
    chatWindow = USERINTERFACE->LoadLayout( RESOURCECACHE->GetResource<XMLFile>( "UI/ChatWindow.xml" ), style );

    //Get Chat History Element
    auto chatHistory = chatWindow->GetChildDynamicCast<ScrollView>( "ChatHistory", true );

    //Create Content Element and set it as Content
    SharedPtr<UIElement> chatContentElement( chatHistory->CreateChild<UIElement>() );
    chatHistory->SetContentElement( chatContentElement );
    chatContentElement->SetLayoutMode( LM_VERTICAL );
    chatContentElement->SetStyleAuto();
    chatContentElement->SetFixedWidth( chatHistory->GetWidth() - 20 );

    //Subscribe Events
    SubscribeToEvent( E_KEYDOWN, URHO3D_HANDLER( ChatHandler, HandleKeyDown ) );
    SubscribeToEvent( chatWindow->GetChild( "SendButton", true ), E_RELEASED, URHO3D_HANDLER( ChatHandler, HandleSendButtonPressed ) );

    return true;
}

void ChatHandler::Load()
{
    //Add Window for UI
    USERINTERFACE->GetRoot()->AddChild( chatWindow );
}

void ChatHandler::AddMessage( const String& message )
{
    //Get Chat History Element
    auto chatHistory = chatWindow->GetChildDynamicCast<ScrollView>( "ChatHistory", true );

    if( chatHistory )
    {
        SharedPtr<Text> messageText( new Text( context_ ) );

        messageText->SetFont( RESOURCECACHE->GetResource<Font>( "Fonts/Anonymous Pro.ttf" ), 12 );
        messageText->SetColor( Color::WHITE );
        messageText->SetWordwrap( true );
        messageText->SetText( message );
        chatHistory->GetContentElement()->AddChild( messageText );
        chatHistory->GetContentElement()->UpdateLayout();
    }
}

bool ChatHandler::HandleChatGame( Connection* connection, MemoryBuffer& message )
{
    auto messageChat = message.ReadString();

    AddMessage( messageChat );

    return true;
}

void ChatHandler::SendMessage( const String& message )
{
    if( message.Length() )
    {
        //Command?
        if( message[0] == '/' )
        {
            auto chatCommand = message.Substring( 0, message.Find( ' ' ) );

            if( chatCommand.Compare( "/SetAnim", false ) == 0 )
            {
                if( String param1; GetParameterString( message, 1, param1 ) )
                {
                    CHARACTERHANDLER->ChangeAnimation( ToInt( param1 ) );
                    AddMessage( "> Animation (" + param1 + ")" );
                }
                else
                    AddMessage( "> Use: /SetAnim <animID>" );
            }
        }
        else
        {
        }

        //Send Message for Game Server
        VectorBuffer chatGameMsg;
        chatGameMsg.WriteString( message );
        CONNECTIONG->Send( MSGID_ChatGame, true, true, chatGameMsg );
    }
}

void ChatHandler::HandleKeyDown( StringHash eventType, VariantMap& eventData )
{
    using namespace KeyDown;

    if( eventData[P_KEY] == KEY_RETURN )
    {
        auto lineEditInput = chatWindow->GetChildStaticCast<LineEdit>( "Input", true );

        //Send Message
        if( lineEditInput->HasFocus() )
        {
            if( lineEditInput->GetText().Length() )
            {
                SendMessage( lineEditInput->GetText() );
                lineEditInput->SetText( "" );
            }
            else
                lineEditInput->SetFocus( false );
        }
        else
        {
            lineEditInput->SetFocus( true );
        }
    }
}

void ChatHandler::HandleSendButtonPressed( StringHash eventType, VariantMap& eventData )
{
    auto lineEditInput = chatWindow->GetChildStaticCast<LineEdit>( "Input", true );

    if( lineEditInput )
    {
        SendMessage( lineEditInput->GetText() );
        lineEditInput->SetText( "" );
    }
}
