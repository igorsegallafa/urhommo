#pragma once

//Interfaces
#include "HandlerImpl.h"
#include "ManagerImpl.h"

//Core
#include "Entity.h"
#include "User.h"
#include "Character.h"
#include "Animation.h"

//Definitions
#include "MessageDef.h"
#include "LoginDef.h"
#include "MapDef.h"

//Implementations
#include "MessageHandler.h"

namespace Shared
{
extern bool isGameRunning;
static void SetIsGameRunning( bool b ) { isGameRunning = b; }

static void RegisterLibrary( Context* context )
{
    context->RegisterFactory<Core::Entity>();
    context->RegisterFactory<Core::Character>();
    context->RegisterFactory<Core::AnimationEntity>();
}
};