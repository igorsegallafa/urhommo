#pragma once

#define GAMEMANAGER       GetSubsystem<Manager::Game>()
#define GAMEHANDLER       GetSubsystem<Handler::Game>()
#define INPUT             GetSubsystem<Input>()

//Shared:
#include "../Shared/IShared.h"

#include "GameManager.h"
#include "GameHandler.h"