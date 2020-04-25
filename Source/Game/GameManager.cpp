#include "PrecompiledHeader.h"
#include "GameManager.h"

void GameManager::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new GameManager( context ) );
}

GameManager::GameManager( Context* context ) :
    ManagerImpl( context )
{
    IMPL_MANAGER( ScreenManager );
    IMPL_MANAGER( CameraManager );
    IMPL_MANAGER( MapManager );
    IMPL_MANAGER( ConfigManager );
    IMPL_MANAGER( EquipmentManager );
}

GameManager::~GameManager()
{
    for( auto& manager : managers_ )
        delete manager.second_;

    managers_.Clear();
}

bool GameManager::Init()
{
    for( const auto& manager : managers_ )
        manager.second_->Init();

    return true;
}

void GameManager::UnInit()
{
    for( const auto& manager : managers_ )
        manager.second_->UnInit();
}