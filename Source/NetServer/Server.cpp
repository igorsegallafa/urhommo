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

void Server::RegisterLibrary( Context* context_ )
{
    context_->RegisterSubsystem( new Server( context_ ) );
    context_->RegisterSubsystem( new Handler::Message( context_ ) );
}

Server::Server( Context* context_ ) : 
    Object( context_ ), 
    id( 0 ), 
    type( ServerType::Undefined ),
    serverConnection( nullptr ),
    connections{}
{
}

Server::~Server()
{
}

bool Server::Init()
{
    //Subscribe Events
    SubscribeToEvent( E_CLIENTIDENTITY, URHO3D_HANDLER( Server, HandleClientIdentity ) );
    SubscribeToEvent( E_NETWORKMESSAGE, URHO3D_HANDLER( Server, HandleMessage ) );
    SubscribeToEvent( E_SERVERCONNECTED, URHO3D_HANDLER( Server, HandleConnectionStatus ) );
    SubscribeToEvent( E_SERVERDISCONNECTED, URHO3D_HANDLER( Server, HandleConnectionStatus ) );
    SubscribeToEvent( E_CONNECTFAILED, URHO3D_HANDLER( Server, HandleConnectionStatus ) );

    return true;
}

void Server::UnInit()
{
    //Disconnect Net Connections
    for( const auto& connection : connections )
        if( connection.connection )
            connection.connection->Disconnect();

    //Close Server
    GetSubsystem<Network>()->Disconnect();
}

bool Server::Start( ServerType serverType, int index )
{
    auto serverConfig = GetSubsystem<ResourceCache>()->GetResource<JSONFile>( "netserver.json" );

    if( serverConfig )
    {
        auto root = serverConfig->GetRoot();
        auto data = root[ServerTypeToString( serverType )];
        int serverPort = 52010; //Default Port
        int maxConnections = 128;   //Default Max Connections

        //Game Server has many servers, read each one
        if( serverType == ServerType::Game )
        {
            int i = 0;
            auto servers = data.GetArray();
            for( const auto& value : servers )
            {
                if( i == index )
                {
                    serverPort = value["port"].GetInt();
                    maxConnections = value["maxConnections"].GetInt();
                    id = index;
                    break;
                }

                i++;
            }
        }
        else
        {
            serverPort = data["port"].GetInt();
            maxConnections = data["maxConnections"].GetInt();
        }

        //Curent Server Type
        type = serverType;

        //Start Server
        return GetSubsystem<Network>()->StartServer( serverPort, maxConnections );
    }

    return false;
}

bool Server::Load( ServerType serverType )
{
    auto serverConfig = GetSubsystem<ResourceCache>()->GetResource<JSONFile>( "netserver.json" );

    if( serverConfig )
    {
        auto root = serverConfig->GetRoot();
        auto data = root[ServerTypeToString(serverType)];

        //Game Server has many servers, read each one
        if( serverType == ServerType::Game )
        {
            auto servers = data.GetArray();
            for( const auto& value: servers )
            {
                NetConnection netConnection;
                netConnection.connection = nullptr;
                netConnection.name = value["name"].GetString();
                netConnection.ip = value["ip"].GetString();
                netConnection.port = value["port"].GetInt();
                netConnection.serverType = serverType;
                connections.Push( netConnection );
            }
        }
        else
        {
            NetConnection netConnection;
            netConnection.connection = nullptr;
            netConnection.name = data["name"].GetString();
            netConnection.ip = data["ip"].GetString();
            netConnection.port = data["port"].GetInt();
            netConnection.serverType = serverType;
            connections.Push( netConnection );
        }
    
        return true;
    }

    return false;
}

bool Server::ConnectAll()
{
    //Server already connected
    if( !serverConnection )
        return false;

    for( const auto& connectionInfo : connections )
    {
        //Prepare Variant Map of identity for Connection
        VariantMap identity;
        identity[P_SERVERTYPE] = (int)type;
        identity[P_SERVERID] = id;

        //Make server connection
        if( serverConnection = GetSubsystem<Network>()->Connect( connectionInfo.ip, connectionInfo.port, nullptr, identity ); serverConnection )
            return true;
    }

    return false;
}

NetConnection* Server::GetConnection( ServerType serverType, int index ) const
{
    int i = 0;
    for( auto connection : connections )
    {
        //Found!
        if( connection.serverType == serverType && i == index )
            return &connection;

        i++;
    }

    return nullptr;
}

void Server::HandleClientIdentity( StringHash eventType, VariantMap& eventData )
{
    Variant outServerType, outServerID;
    if( eventData.TryGetValue( P_SERVERTYPE, outServerType ) && eventData.TryGetValue( P_SERVERID, outServerID ) )
    {
        //Check if already have the connection
        if( GetConnection( (ServerType)outServerType.GetInt(), outServerID.GetInt() ) )
            eventData[ClientIdentity::P_ALLOW] = false;
        else
        {
            NetConnection netConnection;
            netConnection.connection = nullptr;
            netConnection.id = outServerID.GetInt();
            netConnection.serverType = (ServerType)outServerType.GetInt();
            netConnection.connection = static_cast<Connection*>(eventData[ClientIdentity::P_CONNECTION].GetPtr());
            connections.Push( netConnection );

            //TODO: Validate the connection with specific password
        }
    }
}

void Server::HandleMessage( StringHash eventType, VariantMap& eventData )
{
    GetSubsystem<Handler::Message>()->HandleMessage( eventType, eventData );
}

void Server::HandleConnectionStatus( StringHash eventType, VariantMap& eventData )
{
    //Server Connected
    if( eventType == E_SERVERCONNECTED )
    {
    }
    //Server Disconnected
    else if( eventType == E_SERVERDISCONNECTED )
    {
        serverConnection = nullptr;
    }
    //Server Connect Failed
    else if( eventType == E_CONNECTFAILED )
    {
    }
}
}