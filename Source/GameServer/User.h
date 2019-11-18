#pragma once

#include "../Shared/User.h"
 
namespace Core
{
class User : public Shared::Core::User
{
public:
    //! Constructor.
    User( Context* context_ );

    //! Deconstructor.
    ~User();
};
}