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
}

ServerManager::~ServerManager()
{
    for( auto& p : managers )
        delete p.second_;

    managers.Clear();
}

bool ServerManager::Init()
{
    for( const auto& m : managers )
        m.second_->Init();

    return true;
}

void ServerManager::UnInit()
{
    for( const auto& m : managers )
        m.second_->UnInit();
}