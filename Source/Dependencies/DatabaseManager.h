#pragma once

#include "DatabaseConnection.h"

enum class DatabaseConn
{
    AoR,
}; URHO3D_FLAGSET( DatabaseConn, DatabaseConnFlags );

class DatabaseManager : public ManagerImpl
{
    URHO3D_OBJECT( DatabaseManager, ManagerImpl );
public:
    //! Constructor.
    DatabaseManager( Context* context );

    //! Deconstructor.
    ~DatabaseManager();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();

    DatabaseConnection* Get( const DatabaseConnFlags& dbConn );
private:
    HashMap<DatabaseConnFlags, SharedPtr<DatabaseConnection>> dbConnections_;
};