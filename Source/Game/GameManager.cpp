#include "PrecompiledHeader.h"
#include "GameManager.h"

namespace Manager
{
void Game::RegisterLibrary( Context* context )
{
    context->RegisterSubsystem( new Game( context ) );
}

Game::Game( Context* context ) :
    Impl( context )
{
    screenManager = new Screen( context );
    cameraManager = new Camera( context );
}

Game::~Game()
{
    screenManager = nullptr;
    cameraManager = nullptr;
}

bool Game::Init()
{
    screenManager->Init();
    cameraManager->Init();

    return true;
}

void Game::UnInit()
{
    screenManager->UnInit();
    cameraManager->UnInit();
}
}