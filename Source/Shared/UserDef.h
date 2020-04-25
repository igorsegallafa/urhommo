#pragma once

enum class LoginStatus
{
    None            = 0,

    Successful      = 1 << 0,
    AlreadyIngame   = 1 << 1,
    Banned          = 1 << 2,
    WrongAccount    = 1 << 3,
    WrongPassword   = 1 << 4,
}; URHO3D_FLAGSET( LoginStatus, LoginStatusFlags );