#pragma once

#define GAMEMANAGER         GetSubsystem<GameManager>()
#define GAMEHANDLER         GetSubsystem<GameHandler>()
#define INPUT               GetSubsystem<Input>()
#define USERINTERFACE       GetSubsystem<UI>()
#define RESOURCECACHE       GetSubsystem<ResourceCache>()
#define RENDERER            GetSubsystem<Renderer>()
#define GRAPHICS            GetSubsystem<Graphics>()
#define NETWORK             GetSubsystem<Urho3D::Network>()
#define NOTIFICATIONTEXT    GetSubsystem<NotificationText>()

//Shared:
#include "../Shared/IShared.h"

#include "UI.h"

#include "GameManager.h"
#include "GameHandler.h"