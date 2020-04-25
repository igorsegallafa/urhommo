#include "PrecompiledHeader.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager( Context* context ) :
    ManagerImpl( context )
{
    //Default Game Configurations
    config_.screenResolutionWidth = 1024;
    config_.screenResolutionHeight = 768;
    config_.windowedMode = true;
    config_.startMaximized = true;
}

ConfigManager::~ConfigManager()
{
}

bool ConfigManager::Load( const String& filePath )
{
    File file( context_, filePath );
    
    if( file.IsOpen() )
    {
        JSONFile gameConfig( context_ );
        gameConfig.BeginLoad( file );

        auto root = gameConfig.GetRoot();

        auto screen = root["screen"];
        {
            config_.screenResolutionWidth = screen["width"].GetInt();
            config_.screenResolutionHeight = screen["height"].GetInt();
            config_.windowedMode = screen["windowed"].GetBool();
            config_.startMaximized = screen["startMaximized"].GetBool();
        }

        file.Close();
        return true;
    }

    return false;
}