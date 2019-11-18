#pragma once

#define SERVERMANAGER       GetSubsystem<Manager::Server>()
#define SERVERHANDLER       GetSubsystem<Handler::Server>()
#define NETSERVER           GetSubsystem<Net::Server>()

//Shared:
#include "../Shared/Message.h"

//Core:
#include "User.h"

#include "ManagerImpl.h"

#include "ServerManager.h"
#include "ServerHandler.h"