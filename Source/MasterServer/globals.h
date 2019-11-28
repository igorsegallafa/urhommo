#pragma once

#define SERVERMANAGER       GetSubsystem<ServerManager>()
#define SERVERHANDLER       GetSubsystem<ServerHandler>()

//Shared:
#include "../Shared/IShared.h"

//Core:
#include "User.h"

#include "ServerManager.h"
#include "ServerHandler.h"