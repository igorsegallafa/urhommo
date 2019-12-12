#pragma once

//Interfaces
#include "HandlerImpl.h"
#include "ManagerImpl.h"

//Core
#include "Entity.h"
#include "User.h"
#include "Character.h"

//Definitions
#include "MessageDef.h"
#include "LoginDef.h"
#include "MapDef.h"

#include "MessageHandler.h"

namespace Shared
{
static void RegisterLibrary( Context* context )
{
    context->RegisterFactory<Core::Entity>();
    context->RegisterFactory<Core::Character>();
}
};