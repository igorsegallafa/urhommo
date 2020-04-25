#pragma once

using DatabaseResult = pqxx::result;

class DatabaseTransaction : public Object
{
    URHO3D_OBJECT( DatabaseTransaction, Object );
public:
    //! Constructor.
    DatabaseTransaction( Context* context, pqxx::work* transaction ) : 
        Object( context ), 
        transaction_( transaction ){}

    //! Deconstructor.
    ~DatabaseTransaction()
    {
        if( transaction_ )
        {
            delete transaction_;
            transaction_ = nullptr;
        }
    }

    //! Commit Transaction.
    void Commit() const{ transaction_->commit(); }

    //! Abort Transaction.
    void Abort() const{ transaction_->abort(); }

    //! Execute a query with parameters.
    template<typename ...Args>
    const DatabaseResult Exec( const String& query, Args&&...args )
    {
        auto result = transaction_->exec_params( query.CString(), std::forward<Args>( args )... );
        transaction_->commit();

        return result;
    }

    //! Execute a prepared statement with parameters.
    template<typename ...Args>
    const DatabaseResult ExecPrepared( const String& name, Args&&...args )
    {
        auto result = transaction_->exec_prepared( name.CString(), std::forward<Args>( args )... );
        transaction_->commit();

        return result;
    }
private:
    pqxx::work* transaction_;
};

class DatabaseConnection : public Object
{
    URHO3D_OBJECT( DatabaseConnection, Object );
public:
    //! Constructor.
    DatabaseConnection( Context* context );

    //! Deconstructor.
    ~DatabaseConnection();

    //! Make a PostgreSQL transaction.
    UniquePtr<DatabaseTransaction> MakeTransaction()
    {
        return MakeUnique<DatabaseTransaction>( context_, new pqxx::work( *dbConnection_ ) );
    }

    //! Prepare statement.
    void Prepare( const String& name, const String& query )
    {
        dbConnection_->prepare( name.CString(), query.CString() );
    }
    
    //! Unprepare statement.
    void Unprepare( const String& name )
    {
        dbConnection_->unprepare( name.CString() );
    }

    /**
     * Execute a query with parameters in a unique transaction
     * @param query String query
     * @param args Parameters
     * @return PostgreSQL query result
     */
    template<typename ...Args>
    const DatabaseResult Exec( const String& query, Args&&...args )
    {
        auto transaction = MakeTransaction();
        auto result = transaction->Exec( query, std::forward<Args>( args )... );

        transaction->Commit();
        return result;
    }

    /**
     * Execute a prepared statement with parameters in a unique transaction
     * @param name Name of prepared statement
     * @param args Parameters
     * @return PostgreSQL query result
     */
    template<typename ...Args>
    const DatabaseResult ExecPrepared( const String& name, Args&&...args )
    {
        auto transaction = MakeTransaction();
        auto result = transaction->Exec( name, std::forward<Args>( args )... );

        transaction->Commit();
        return result;
    }

    /**
     * Create PostgreSQL Connection
     * @param database Database Name
     * @param user Database Username
     * @param password Database Password
     * @param hostname Database Hostname
     */
    void Create( const String& database, const String& user = "postgres", const String& password = "123456", const String& hostname = "localhost" );

    //! Close Database Connection.
    void Close();
private:
    pqxx::connection* dbConnection_;
};