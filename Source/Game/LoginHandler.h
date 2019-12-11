#pragma once

struct ServerInfo
{
    String name;
    String ip;
    unsigned int port;
};

class LoginHandler : public HandlerImpl
{
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
     * Process Account Login into Game Server
     * @param serverIndex Index of Game Server Selected
     */
    void ProcessGameServer( unsigned int serverIndex = 0 );

    /**
     * Process Account Login into Master Server
     */
    void ProcessMasterServer();

    //! Handle Login Data Message.
    bool HandleLoginData( Connection* connection, MemoryBuffer& message );

    //! Handle Game Server Connected.
    bool HandleGameServerConnected( Connection* connection, MemoryBuffer& message );
private:
    Vector<ServerInfo> gameServerList;
    ServerInfo masterServerInfo;
};