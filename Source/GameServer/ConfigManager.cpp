#include "PrecompiledHeader.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager(Context* context) :
    ManagerImpl( context ),
    connection( nullptr )
{
}

ConfigManager::~ConfigManager()
{
    for( auto& connection : netConnections )
        delete connection;

    netConnections.Clear();
}

bool ConfigManager::Init()
{
    Load( Net::ServerType::Game );

    return true;
}

bool ConfigManager::Load( const Net::ServerType& serverType, int id )
{
    auto serverConfig = GetSubsystem<ResourceCache>()->GetResource<JSONFile>( "config.json" );

    if( serverConfig )
    {
        auto root = serverConfig->GetRoot();
        
        auto ReadServerConfig = [=]( auto json, auto type )
        {
            Net::NetConnection* netConnection = new Net::NetConnection();
            netConnection->connection = nullptr;
            netConnection->id = json["id"].GetInt();
            netConnection->name = json["name"].GetString();
            netConnection->ip = json["ip"].GetString();
            netConnection->port = json["port"].GetInt();
            netConnection->maxConnections = json["maxConnections"].GetInt();
            netConnection->serverType = type;
            netConnections.Push( netConnection );

            //Current Server Config
            if( type == serverType && netConnection->id == id )
                connection = netConnection;
        };

        for( auto it = root.Begin(); it != root.End(); ++it )
        {
            auto object = (*it);

            //Read Game Servers Config
            if( object.first_ == ServerTypeToString( Net::ServerType::Game ) )
            {
                auto servers = object.second_.GetArray();

                for( const auto& value : servers )
                    ReadServerConfig( value, Net::ServerType::Game );
            }
            else
                ReadServerConfig( object.second_, Net::ServerTypeFromString( object.first_ ) );
        }

        return true;
    }

    return false;
}

Net::NetConnection* ConfigManager::GetNetConfig( const Net::ServerType& serverType, int id )
{
    for( auto& connection : netConnections )
    {
        if( connection->serverType == serverType && connection->id == id )
            return connection;
    }

    return nullptr;
}

