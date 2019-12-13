#pragma once

#define SERVERMANAGER       GetSubsystem<ServerManager>()
#define SERVERHANDLER       GetSubsystem<ServerHandler>()
#define NETWORK             GetSubsystem<Network>()

//Shared:
#include "../Shared/IShared.h"

//Core:
#include "User.h"

#include "ServerManager.h"
#include "ServerHandler.h"