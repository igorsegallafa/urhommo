#pragma once

//STD:
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

//Windows:
#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

//Urho3D:
#include <Urho3D/Urho3DAll.h>

//Net Server:
#include "../NetServer/INetServer.h"

using namespace Urho3D;

//Globals:
#include "globals.h"