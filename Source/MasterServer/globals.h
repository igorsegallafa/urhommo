#pragma once

#define SERVERMANAGER       GetSubsystem<Manager::Server>()
#define SERVERHANDLER       GetSubsystem<Handler::Server>()

//Shared:
#include "../Shared/IShared.h"

//Core:
#include "User.h"

#include "HandlerImpl.h"
#include "ManagerImpl.h"

#include "ServerManager.h"
#include "ServerHandler.h"