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

    return ServerType::Undefined;
}

Server::Server( Context* context ) :
    Object( context ),
    serverConnection( nullptr ),
    netConnections{}
{
}

Server::~Server()
{
}

bool Server::Init()
{
    //Subscribe Events
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
    for( const auto& connection : netConnections )
        if( connection->connection )
            connection->connection->Disconnect();

    //Close Server
    GetSubsystem<Network>()->Disconnect();
    GetSubsystem<Network>()->StopServer();
}

bool Server::Start( NetConnection* netConnection )
{
    if( netConnection )
    {
        currentNetConnection = netConnection;

        //Start Server
        return GetSubsystem<Network>()->StartServer( currentNetConnection->port, currentNetConnection->maxConnections );
    }

    return false;
}

bool Server::Load( NetConnection* netConnection )
{
    netConnections.Push( netConnection );
    return true;
}

bool Server::ConnectAll()
{
    //Server already connected
    if( serverConnection )
        return false;

    for( const auto& connectionInfo : netConnections )
    {
        //Prepare Variant Map of identity for Connection
        VariantMap identity;
        identity[P_SERVERTYPE] = (int)currentNetConnection->serverType;
        identity[P_SERVERID] = currentNetConnection->id;

        //Make server connection
        if( connectionInfo->connection = GetSubsystem<Network>()->Connect( connectionInfo->ip, connectionInfo->port, nullptr, identity ) )
        {
            //Set as net connection
            connectionInfo->address = 0;
            connectionInfo->connection->SetIsNetConnection( true );
            return true;
        }
    }

    return false;
}

NetConnection* Server::GetConnection( ServerType serverType, int index ) const
{
    int i = 0;
    for( auto connection : netConnections )
    {
        //Found!
        if( connection->serverType == serverType && i == index )
            return connection;

        i++;
    }

    return nullptr;
}

void Server::Send( ServerType serverType, int msgID, bool reliable, bool inOrder, const VectorBuffer& msg )
{
    for( const auto& connection : netConnections )
        if( connection->connection && connection->serverType == serverType )
            connection->connection->Send( msgID, reliable, inOrder, msg );
}

void Server::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
    if( Variant outServerType, outServerID; eventData.TryGetValue( P_SERVERTYPE, outServerType ) && eventData.TryGetValue( P_SERVERID, outServerID ) )
    {
        //Check if already have the connection
        if( GetConnection( (ServerType)outServerType.GetInt(), outServerID.GetInt() ) )
            eventData[ClientIdentity::P_ALLOW] = false;
        else
        {
            NetConnection* netConnection = new NetConnection();
            netConnection->id = outServerID.GetInt();
            netConnection->serverType = (ServerType)outServerType.GetInt();
            netConnection->connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());
            netConnection->address = netConnection->connection->GetAddressOrGUIDHash();
            netConnections.Push( netConnection );

            URHO3D_LOGINFOF( "[Net Server] New connection established from '%s'", ServerTypeToString( (ServerType)outServerType.GetInt() ) );

            //Set as net connection
            netConnection->connection->SetIsNetConnection( true );

            //TODO: Validate the connection with specific password
        }
    }
}

void Server::HandleClientDisconnect( StringHash eventType, VariantMap& eventData )
{
    auto connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());

    for( auto it = netConnections.Begin(); it != netConnections.End(); ++it )
    {
        if( (*it)->connection == connection )
        {
            netConnections.Erase( it );
            break;
        }
    }
}

void Server::HandleConnectionStatus( StringHash eventType, VariantMap& eventData )
{
    //Server Connected
    if( eventType == E_SERVERCONNECTED )
    {
        //Update all Server Connections Address
        for( auto& connectionInfo : netConnections )
            if( connectionInfo->connection )
                if( connectionInfo->connection->IsConnectPending() == false && connectionInfo->address == 0 )
                    connectionInfo->address = connectionInfo->connection->GetAddressOrGUIDHash();
    }
    //Server Disconnected
    else if( eventType == E_SERVERDISCONNECTED )
    {
        for( auto it = netConnections.Begin(); it != netConnections.End(); ++it )
        {
            if( (*it)->address == eventData[ServerDisconnected::P_ADDRESS].GetUInt() )
            {
                netConnections.Erase( it );
                break;
            }
        }
    }
    //Server Connect Failed
    else if( eventType == E_CONNECTFAILED )
    {
    }
}
}