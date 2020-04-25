#pragma once

struct Config
{
    int screenResolutionWidth;
    int screenResolutionHeight;
    bool windowedMode;
    bool startMaximized;
};

class ConfigManager : public ManagerImpl
{
    URHO3D_OBJECT( ConfigManager, ManagerImpl );
public:
    //! Constructor.
    ConfigManager( Context* context );

    //! Deconstructor.
    ~ConfigManager();

    //! Load Config File.
    bool Load( const String& filePath );

    //! Configuration Getter.
    const Config& Get() const{ return config_; }
private:
    Config config_;
};