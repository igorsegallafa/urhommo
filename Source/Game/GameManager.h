#pragma once

namespace Manager
{
class Game : public Impl
{
public:
    //! Register Object Factory.
    static void RegisterLibrary( Context* context );

    //! Constructor.
    Game( Context* context );

    //! Deconstructor.
    ~Game();

    //! Initialize Object.
    bool Init();

    //! UnInitialize Object.
    void UnInit();
};
}
