#pragma once

//STD:
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>

//Urho3D:
#include <Urho3D/Urho3DAll.h>

using namespace Urho3D;

namespace Shared
{
extern bool isGameRunning;
inline bool IsGameRunning(){ return isGameRunning; }
};