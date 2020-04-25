#include "PrecompiledHeader.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager(Context* context) :
    ManagerImpl( context ),
    connection_( nullptr ),
    parameters_{}
{
}

ConfigManager::~ConfigManager()
{
    for( auto& connection : netConnections_ )
        delete connection;

    netConnections_.Clear();
}

bool ConfigManager::Init()
{
    ParseParameters();
    Load( Net::ServerType::Game, parameters_.Find( CP_SERVERID ) != parameters_.End() ? parameters_[CP_SERVERID].GetInt() : 0 );

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
            netConnection->connection_ = nullptr;
            netConnection->id_ = json["id"].GetInt();
            netConnection->name_ = json["name"].GetString();
            netConnection->ip_ = json["ip"].GetString();
            netConnection->port_ = json["port"].GetInt();
            netConnection->maxConnections_ = json["maxConnections"].GetInt();
            netConnection->serverType_ = type;
            netConnections_.Push( netConnection );

            //Current Server Config
            if( type == serverType && netConnection->id_ == id )
                connection_ = netConnection;
        };

        for( auto it = root.Begin(); it != root.End(); ++it )
        {
            auto object = (*it);

            //Read Game Servers Config
            if( object.first_ == ServerTypeToString( Net::ServerType::Game ) || object.first_ == ServerTypeToString( Net::ServerType::World ) )
            {
                auto servers = object.second_.GetArray();

                for( const auto& value : servers )
                    ReadServerConfig( value, Net::ServerTypeFromString( object.first_ ) );
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
    for( auto& connection : netConnections_ )
        if( connection->serverType_ == serverType && connection->id_ == id )
            return connection;

    return nullptr;
}

void ConfigManager::ParseParameters()
{
    auto arguments = GetArguments();

    for( unsigned i = 0; i < arguments.Size(); ++i )
    {
        if( arguments[i].Length() > 1 && arguments[i][0] == '-' )
        {
            String argument = arguments[i].Substring( 1 ).ToLower();
            String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

            if( argument == "serverid" && !value.Empty() )
            {
                parameters_[CP_SERVERID] = ToInt( value );
                ++i;
            }
        }
    }
}