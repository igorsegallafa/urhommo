#pragma once

#define SERVERMANAGER       GetSubsystem<ServerManager>()
#define SERVERHANDLER       GetSubsystem<ServerHandler>()
#define INPUT               GetSubsystem<Input>()
#define USERINTERFACE       GetSubsystem<UI>()
#define RESOURCECACHE       GetSubsystem<ResourceCache>()
#define RENDERER            GetSubsystem<Renderer>()
#define GRAPHICS            GetSubsystem<Graphics>()
#define NETWORK             GetSubsystem<Urho3D::Network>()

//Shared:
#include "../Shared/IShared.h"

//Core:
#include "User.h"

#include "ServerManager.h"
#include "ServerHandler.h"