#include "PrecompiledHeader.h"
#include "LoginHandler.h"

LoginHandler::LoginHandler( Context* context ) :
    HandlerImpl( context )
{
}

LoginHandler::~LoginHandler()
{
}

const LoginStatusFlags LoginHandler::HandleUserValidation( const String& account, const String& password, int& outUserID )
{
    LoginStatusFlags loginStatus = LoginStatus::None;
    auto db = DATABASEMANAGER->Get( DatabaseConn::AoR );
    
    outUserID = -1;

    if( db )
    {
        String query =  "SELECT id, "
                        "password = $2 AS ValidPassword "
                        "FROM aor_user.users "
                        "WHERE account = $1 LIMIT 1;";

        auto results = db->Exec( query, account.CString(), password.CString() );
        
        if( auto result = results[0]; results.size() )
        {
            if( result["ValidPassword"].as<bool>() == false )
                loginStatus = LoginStatus::WrongPassword;
            else
                outUserID = result["id"].as<int>();
        }
        else
            loginStatus = LoginStatus::WrongAccount;
    }

    return loginStatus;
}

const LoginStatusFlags LoginHandler::HandleUserLogin( User* user )
{
    LoginStatusFlags loginStatus = LoginStatus::None;

    if( IsAccountLogged( user ) )
        loginStatus |= LoginStatus::AlreadyIngame;

   /**
    * Example of how this should works
    *
    *if( IsAccountBanned( user ) )
    *    loginStatus |= LoginStatus::Banned;
    *
    *if( IsAccountGameMaster( user ) )
    *    loginStatus |= LoginStatus::GameMaster;
    */

    //Successful Login
    if( loginStatus == LoginStatus::None )
        loginStatus |= LoginStatus::Successful;

    return loginStatus;
}

void LoginHandler::ProcessLoginResponse( const LoginStatusFlags& loginStatus, User* user )
{
    //Login Response Message
    VectorBuffer loginDataMsg;
    loginDataMsg.WriteInt( loginStatus );

    if( loginStatus & LoginStatus::Successful )
    {
        //Get Total World Servers
        auto worldServersFound = CONFIGMANAGER->GetNetConnections( Net::ServerType::World );

        //Write Total of Game Servers
        loginDataMsg.WriteInt( worldServersFound.Size() );

        //Write World Servers Info
        for( const auto& worldServer : worldServersFound )
        {
            loginDataMsg.WriteString( worldServer->name_ );
            loginDataMsg.WriteString( worldServer->ip_ );
            loginDataMsg.WriteInt( worldServer->port_ );
            loginDataMsg.WriteBool( worldServer->connection_ != nullptr );   //World Server is online?

            auto gameServerInfo = CONFIGMANAGER->GetNetConfig( Net::ServerType::Game, worldServer->id_ );
            loginDataMsg.WriteString( gameServerInfo->ip_ );
            loginDataMsg.WriteInt( gameServerInfo->port_ );
        }

        //Write Character List
        auto characters = GetCharactersFromAccount( user->id_ );
        loginDataMsg.WriteUInt( characters.Size() );

        for( const auto& character : characters )
        {
            loginDataMsg.WriteString( character.name );
            loginDataMsg.WriteUInt( character.level );
            loginDataMsg.WriteInt( character.characterClass );
            loginDataMsg.WriteInt( character.armorId );
            loginDataMsg.WriteInt( character.headId );
        }

        //Write Master Server Info
        auto masterServerInfo = CONFIGMANAGER->GetNetConfig( Net::ServerType::Master );
        loginDataMsg.WriteString( masterServerInfo->ip_ );
        loginDataMsg.WriteInt( masterServerInfo->port_ );
    }

    //Send Message
    user->connection_->Send( MSGID_LoginData, true, true, loginDataMsg );
}

bool LoginHandler::IsAccountLogged( User* user )
{
    if( const auto& foundUser = USERMANAGER->GetUser( user->accountName_ ); foundUser )
        return foundUser != user;

    return false;
}

Vector<CharacterInfo>& LoginHandler::GetCharactersFromAccount( int userId )
{
    Vector<CharacterInfo> ret;
    auto db = DATABASEMANAGER->Get( DatabaseConn::AoR );

    if( db )
    {
        String query =  "SELECT name, level, class, armor_id, head_id "
                        "FROM aor_user.characters "
                        "WHERE user_id = $1;";

        auto results = db->Exec( query, userId );

        for( const auto& result : results )
        {
            CharacterInfo characterInfo;
            characterInfo.name = result["name"].as<std::string>().c_str();
            characterInfo.level = result["level"].as<int>();
            characterInfo.characterClass = result["class"].as<int>();
            characterInfo.armorId = result["armor_id"].as<int>();
            characterInfo.headId = result["head_id"].as<int>();
            ret.Push( characterInfo );
        }
    }

    return ret;
}
