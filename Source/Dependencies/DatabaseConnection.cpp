#include "PrecompiledHeader.h"
#include "DatabaseConnection.h"

DatabaseConnection::DatabaseConnection( Context* context ) :
    Object( context ),
    dbConnection_( nullptr )
{
}

DatabaseConnection::~DatabaseConnection()
{
    if( dbConnection_ )
    {
        delete dbConnection_;
        dbConnection_ = nullptr;
    }
}

void DatabaseConnection::Create( const String& database, const String& user, const String& password, const String& hostname )
{
    const String connectionString = "postgresql://" + user + ":" + password + "@" + hostname + "/" + database;
    dbConnection_ = new pqxx::connection( connectionString.CString() );
}

void DatabaseConnection::Close()
{
    if( dbConnection_ )
        dbConnection_->disconnect();
}
