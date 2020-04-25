#include "PrecompiledHeader.h"
#include "DatabaseManager.h"

static HashMap<DatabaseConnFlags, String> databaseAsString = {
    {DatabaseConn::AoR, "aor"},
};

DatabaseManager::DatabaseManager( Context* context ) : 
    ManagerImpl( context )
{
}

DatabaseManager::~DatabaseManager()
{
    dbConnections_.Clear();
}

bool DatabaseManager::Init()
{
    const String& hostname = "localhost";
    const String& user = "postgres";
    const String& password = "123456";

    //Create Connections Pointer
    dbConnections_[DatabaseConn::AoR] = MakeShared<DatabaseConnection>( context_ );

    //Initialize Database Connections
    for( const auto& dbConnection : dbConnections_ )
        dbConnection.second_->Create( databaseAsString[dbConnection.first_], user, password, hostname );

    return true;
}

void DatabaseManager::UnInit()
{
    for( const auto& dbConnection : dbConnections_ )
        dbConnection.second_->Close();
}

DatabaseConnection* DatabaseManager::Get( const DatabaseConnFlags& dbConn )
{
    auto it = dbConnections_.Find( dbConn );

    if( it != dbConnections_.End() )
        return it->second_;

    return nullptr;
}