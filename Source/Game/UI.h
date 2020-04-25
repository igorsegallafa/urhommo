#pragma once

#include "NotificationText.h"

namespace UserInterface
{
static void RegisterLibrary( Context* context )
{
    NotificationText::RegisterObject( context );
}
};
