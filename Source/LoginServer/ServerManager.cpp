#include "PrecompiledHeader.h"
#include "ServerManager.h"

void ServerManager::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new ServerManager( context ) );
}

ServerManager::ServerManager( Context* context ) :
    ManagerImpl( context )
{
    IMPL_MANAGER( UserManager );
    IMPL_MANAGER( ConfigManager );
    IMPL_MANAGER( DatabaseManager );
}

ServerManager::~ServerManager()
{
    for( auto& manager : managers_ )
        delete manager.second_;

    managers_.Clear();
}

bool ServerManager::Init()
{
    for( const auto& manager : managers_ )
        manager.second_->Init();

    return true;
}

void ServerManager::UnInit()
{
    for( const auto& manager : managers_ )
        manager.second_->UnInit();
}