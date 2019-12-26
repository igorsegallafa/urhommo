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
}

GameManager::~GameManager()
{
    for( auto& p : managers )
        delete p.second_;

    managers.Clear();
}

bool GameManager::Init()
{
    for( const auto& m : managers )
        m.second_->Init();

    return true;
}

void GameManager::UnInit()
{
    for( const auto& m : managers )
        m.second_->UnInit();
}