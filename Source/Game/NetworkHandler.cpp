#include "PrecompiledHeader.h"
#include "NetworkHandler.h"

NetworkHandler::NetworkHandler( Context* context ) :
    HandlerImpl( context ),
    Handler::Message(),
    loginServerConnection_( nullptr ),
    masterServerConnection_( nullptr ),
    gameServerConnection_( nullptr )
{
}

NetworkHandler::~NetworkHandler()
{
}

bool NetworkHandler::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_SERVERCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleServerConnected ) );
    SubscribeToEvent( E_SERVERDISCONNECTED, URHO3D_HANDLER( NetworkHandler, HandleServerDisconnected ) );
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( NetworkHandler, HandleMessage ) );

    //Handlers
    Handle( MSGID_LoginData ).Process( MESSAGE_HANDLER( LoginHandler, HandleLoginData ) );
    Handle( MSGID_GameServerConnected ).Process( MESSAGE_HANDLER( LoginHandler, HandleGameServerConnected ) );
    Handle( MSGID_WorldData ).Process( MESSAGE_HANDLER( CharacterHandler, HandleWorldData ) );
    Handle( MSGID_ChatGame ).Process( MESSAGE_HANDLER( ChatHandler, HandleChatGame ) );

    return true;
}

void NetworkHandler::UnInit()
{
    CloseConnections();
}

void NetworkHandler::ConnectLoginServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        loginServerConnection_ = connection;
}

void NetworkHandler::ConnectMasterServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        masterServerConnection_ = connection;
}

void NetworkHandler::ConnectWorldServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        worldServerConnection_ = connection;
}

void NetworkHandler::ConnectGameServer( const String& ip, unsigned int port, VariantMap& identity )
{
    if( auto connection = Connect( ip, port, identity ); connection )
        gameServerConnection_ = connection;
}

void NetworkHandler::CloseConnections()
{
    NETWORK->Disconnect();
}

void NetworkHandler::CloseLoginServer()
{
    if( loginServerConnection_ )
    {
        loginServerConnection_->Disconnect();
        loginServerConnection_ = nullptr;
    }
}

void NetworkHandler::CloseMasterServer()
{
    if( masterServerConnection_ )
    {
        masterServerConnection_->Disconnect();
        masterServerConnection_ = nullptr;
    }
}

void NetworkHandler::CloseWorldServer()
{
    if( worldServerConnection_ )
    {
        worldServerConnection_->Disconnect();
        worldServerConnection_ = nullptr;
    }
}

void NetworkHandler::CloseGameServer()
{
    if( gameServerConnection_ )
    {
        gameServerConnection_->Disconnect();
        gameServerConnection_ = nullptr;
    }
}

Connection* NetworkHandler::Connect( const String& ip, unsigned int port, VariantMap& identity )
{
    //Make server connection
    if( auto serverConnection = NETWORK->Connect( ip, port, nullptr, identity ); serverConnection )
        return serverConnection;

    return nullptr;
}

void NetworkHandler::HandleServerConnected( StringHash eventType, VariantMap& eventData )
{
    using namespace ServerConnected;
    auto connection = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());

    if( connection == gameServerConnection_ )
        LOGINHANDLER->ProcessMasterServer();
    else if( connection == worldServerConnection_ )
        LOGINHANDLER->ProcessGameServer();
}

void NetworkHandler::HandleServerDisconnected( StringHash eventType, VariantMap& eventData )
{
    using namespace ServerDisconnected;

    if( (loginServerConnection_) && loginServerConnection_->GetAddressOrGUIDHash() == eventData[P_ADDRESS].GetInt() )
        loginServerConnection_ = nullptr;
    else if( (gameServerConnection_) && gameServerConnection_->GetAddressOrGUIDHash() == eventData[P_ADDRESS].GetInt() )
        gameServerConnection_ = nullptr;
    else if( (worldServerConnection_) && worldServerConnection_->GetAddressOrGUIDHash() == eventData[P_ADDRESS].GetInt() )
        worldServerConnection_ = nullptr;
    else if( (masterServerConnection_) && masterServerConnection_->GetAddressOrGUIDHash() == eventData[P_ADDRESS].GetInt() )
        masterServerConnection_ = nullptr;
    
    if( SCREENMANAGER->GetActiveScreenType() == ScreenType::World )
    {
        //Disconnect all servers and go back to Login Screen
        CloseConnections();
        SCREENMANAGER->ChangeScreen( ScreenType::Login );
    }
}
