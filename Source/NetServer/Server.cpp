#include "PrecompiledHeader.h"
#include "Server.h"

namespace Net
{
const char* ServerTypeToString( const ServerType& serverType )
{
    switch( serverType )
    {
        case ServerType::Master:
            return "masterserver";
        case ServerType::Login:
            return "loginserver";
        case ServerType::Game:
            return "gameserver";
        case ServerType::World:
            return "worldserver";
    }

    return "unknown";
}

ServerType ServerTypeFromString( const String& str )
{
    if( str.Compare( "masterserver", false ) == 0 )
        return ServerType::Master;
    else if( str.Compare( "loginserver", false ) == 0 )
        return ServerType::Login;
    else if( str.Compare( "gameserver", false ) == 0 )
        return ServerType::Game;
    else if( str.Compare( "worldserver", false ) == 0 )
        return ServerType::World;

    return ServerType::Undefined;
}

Server::Server( Context* context ) :
    Object( context ),
    currentNetConnection_( nullptr ),
    netConnections_{}
{
}

Server::~Server()
{
}

bool Server::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_UPDATE, URHO3D_HANDLER( Server, HandleUpdate ) );
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( Server, HandleClientIdentity ) );
    SubscribeToEvent( E_CLIENTDISCONNECTED, URHO3D_HANDLER( Server, HandleClientDisconnect ) );
    SubscribeToEvent( E_SERVERCONNECTED, URHO3D_HANDLER( Server, HandleConnectionStatus ) );
    SubscribeToEvent( E_SERVERDISCONNECTED, URHO3D_HANDLER( Server, HandleConnectionStatus ) );
    SubscribeToEvent( E_CONNECTFAILED, URHO3D_HANDLER( Server, HandleConnectionStatus ) );

    return true;
}

void Server::UnInit()
{
    //Disconnect Net Connections
    for( const auto& connection : netConnections_ )
        if( connection->connection_ )
            connection->connection_->Disconnect();

    //Close Server
    GetSubsystem<Network>()->Disconnect();
    GetSubsystem<Network>()->StopServer();
}

bool Server::Start( NetConnection* netConnection )
{
    if( netConnection )
    {
        currentNetConnection_ = netConnection;

        //Start Server
        return GetSubsystem<Network>()->StartServer( currentNetConnection_->port_, currentNetConnection_->maxConnections_ );
    }

    return false;
}

bool Server::Load( NetConnection* netConnection )
{
    netConnections_.Push( netConnection );
    return true;
}

void Server::ConnectAll()
{
    for( const auto& netConnection : netConnections_ )
        Connect( netConnection );
}

NetConnection* Server::GetConnection( ServerType serverType, int index ) const
{
    for( auto& netConnection : netConnections_ )
        if( netConnection->serverType_ == serverType && netConnection->id_ == index )
            return netConnection;

    return nullptr;
}

void Server::Send( ServerType serverType, int msgID, bool reliable, bool inOrder, const VectorBuffer& msg )
{
    for( const auto& netConnection : netConnections_ )
        if( netConnection->connection_ && netConnection->serverType_ == serverType )
            netConnection->connection_->Send( msgID, reliable, inOrder, msg );
}

void Server::HandleUpdate( StringHash eventType, VariantMap& eventData )
{
    using namespace Update;
    static float elapsedTime10secs = 0.f;

    //Check every 10 seconds
    if( elapsedTime10secs >= 10.f )
    {
        //Has server to reconnect
        if( reconnectNetConnections_.Size() )
        {
            for( auto& netConnection : reconnectNetConnections_ )
            {
                if( Connect( netConnection ) )
                    URHO3D_LOGINFOF( "NET_SERVER: Trying reconnect to (%s)[%d]", ServerTypeToString( netConnection->serverType_ ), netConnection->id_ );
            }
        }

        elapsedTime10secs = 0.f;
    }

    elapsedTime10secs += eventData[P_TIMESTEP].GetFloat();
}

void Server::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
    if( Variant outServerType, outServerID; eventData.TryGetValue( P_SERVERTYPE, outServerType ) && eventData.TryGetValue( P_SERVERID, outServerID ) )
    {
        //Check if already have the connection
        if( auto connection = GetConnection( (ServerType)outServerType.GetInt(), outServerID.GetInt() ); connection )
        {
            eventData[ClientIdentity::P_ALLOW] = false;
            URHO3D_LOGINFOF( "NET_SERVER: Connection request from (%s)[%d] denied", ServerTypeToString( connection->serverType_ ), connection->id_ );
        }
        else
        {
            NetConnection* netConnection = new NetConnection();
            netConnection->id_ = outServerID.GetInt();
            netConnection->serverType_ = (ServerType)outServerType.GetInt();
            netConnection->connection_ = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());
            netConnection->address_ = netConnection->connection_->GetAddressOrGUIDHash();
            netConnections_.Push( netConnection );

            URHO3D_LOGINFOF( "NET_SERVER: New connection established from (%s)[%d]", ServerTypeToString( netConnection->serverType_ ), netConnection->id_ );

            //Set as net connection
            netConnection->connection_->SetIsNetConnection( true );

            //TODO: Validate the connection with specific password
        }
    }
}

void Server::HandleClientDisconnect( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());

    for( auto it = netConnections_.Begin(); it != netConnections_.End(); ++it )
    {
        if( auto netConnection = (*it); netConnection->connection_ == connection )
        {
            URHO3D_LOGINFOF( "NET_SERVER: Server (%s)[%d] disconnected", ServerTypeToString( (ServerType)netConnection->serverType_ ), netConnection->id_ );
            netConnections_.Erase( it );
            break;
        }
    }
}

void Server::HandleConnectionStatus( StringHash eventType, VariantMap& eventData )
{
    //Server Disconnected
    if( eventType == E_SERVERDISCONNECTED )
    {
        for( auto& netConnection : netConnections_ )
        {
            if( netConnection->address_ == eventData[ServerDisconnected::P_ADDRESS].GetUInt() )
            {
                netConnection->connection_ = nullptr;
                netConnection->address_ = 0;
                URHO3D_LOGINFOF( "NET_SERVER: Disconnected from (%s)[%d]", ServerTypeToString( (ServerType)netConnection->serverType_ ), netConnection->id_ );
                reconnectNetConnections_.Push( netConnection );
                break;
            }
        }
    }
    else 
    {
        for( auto& netConnection : netConnections_ )
        {
            using namespace ClientConnected;

            if( netConnection->connection_ == static_cast<Connection*>(eventData[P_CONNECTION].GetPtr()) )
            {
                //Server Connected
                if( eventType == E_SERVERCONNECTED )
                {
                    bool reconnectedServer = false;

                    //Check if server is reconnecting
                    for( auto it = reconnectNetConnections_.Begin(); it != reconnectNetConnections_.End(); ++it )
                    {
                        if( auto reconnectNetConnection = (*it); reconnectNetConnection == netConnection )
                        {
                            reconnectedServer = true;
                            reconnectNetConnections_.Erase( it );
                            break;
                        }
                    }

                    netConnection->address_ = netConnection->connection_->GetAddressOrGUIDHash();
                    URHO3D_LOGINFOF( "NET_SERVER: %s to (%s)[%d]", (reconnectedServer ? "Reconnected" : "Connected"), ServerTypeToString( (ServerType)netConnection->serverType_ ), netConnection->id_ );
                }
                //Server Connection Failed
                else if( eventType == E_CONNECTFAILED )
                {
                    netConnection->address_ = 0;
                    netConnection->connection_ = nullptr;
                }
            }
        }
    }
}

bool Server::Connect( NetConnection* netConnection )
{
    //We cant connect to a server already connected
    if( auto otherConnection = GetConnection( netConnection->serverType_, netConnection->id_ ); otherConnection )
        if( (otherConnection->connection_) && otherConnection->connection_->IsConnectPending() )
            return false;

    //Prepare Variant Map of identity for Connection
    VariantMap identity;
    identity[P_SERVERTYPE] = (int)currentNetConnection_->serverType_;
    identity[P_SERVERID] = currentNetConnection_->id_;

    //Make server connection
    if( netConnection->connection_ = GetSubsystem<Network>()->Connect( netConnection->ip_, netConnection->port_, nullptr, identity ) )
    {
        //Set as net connection
        netConnection->address_ = 0;
        netConnection->connection_->SetIsNetConnection( true );
        return true;
    }

    return false;
}
}