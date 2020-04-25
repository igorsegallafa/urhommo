#pragma once

class LoginHandler : public HandlerImpl
{
    struct ServerInfo
    {
        String name;
        String ip;
        unsigned int port;
        bool isOnline;
        String gameServerIp;
        unsigned int gameServerPort;
    };

    struct CharacterInfo
    {
        String name;
        unsigned level;
        CharacterClass characterClass;
        int armorId;
        int headId;
    };

    URHO3D_OBJECT( LoginHandler, HandlerImpl );
public:
    //! Constructor.
    LoginHandler( Context* context );

    //! Deconstructor.
    ~LoginHandler();

    /**
     * Process Account Login into Login Server
     * @param account Account Name
     * @param password Password
     */
    void ProcessLogin( const String& account, const String& password );

    /**
     * Process Account Login into World Server
     * @param serverIndex Index of World Server Selected
     */
    void ProcessWorldServer( unsigned int serverIndex = 0 );

    /**
     * Process Account Login into Game Server
     */
    void ProcessGameServer();

    /**
     * Process Account Login into Master Server
     */
    void ProcessMasterServer();

    //! Handle Login Data Message.
    bool HandleLoginData( Connection* connection, MemoryBuffer& message );

    //! Handle Game Server Connected.
    bool HandleGameServerConnected( Connection* connection, MemoryBuffer& message );

    const Vector<CharacterInfo>& GetCharacterList() const{ return characterList_; }
private:
    unsigned int worldServerIndex_;
    Vector<ServerInfo> worldServerList_;
    ServerInfo masterServerInfo_;
    Vector<CharacterInfo> characterList_;
};