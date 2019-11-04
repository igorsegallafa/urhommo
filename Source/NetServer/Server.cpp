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
        case ServerType::Proxy:
            return "proxyserver";
    }

    return "unknown";
}

void Server::RegisterServerLibrary( Context* context_ )
{
    context_->RegisterSubsystem( new Server( context_ ) );
}

Server::Server( Context* context_ ) : Object( context_ )
{
}

Server::~Server()
{
}

bool Server::Load( ServerType serverType )
{
    auto serverConfig = GetSubsystem<ResourceCache>()->GetResource<JSONFile>( "netserver.json" );

    if( serverConfig )
    {
        auto root = serverConfig->GetRoot();
        auto data = root[ServerTypeToString(serverType)];

        //Game Server and Proxy Server has many servers, read each one
        if( serverType == ServerType::Game || serverType == ServerType::Proxy )
        {
            auto servers = data.GetArray();
            for( const auto& value: servers )
            {
                NetConnection netConnection;
                netConnection.connection = nullptr;
                netConnection.id = value["id"].GetInt();
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
            netConnection.id = data["id"].GetInt();
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
}