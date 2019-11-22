#pragma once

#define IMPL_HANDLER(name)  handlers[name::GetTypeStatic()] = new name( context ); 

namespace Handler
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

    //! Handler Getter.
    template<class T>
    inline T* Get()
    {
        auto it = handlers.Find( T::GetTypeStatic() );

        if( it != handlers.End() )
            return static_cast<T*>(it->second_);

        return nullptr;
    }
private:
    HashMap<StringHash, Impl*> handlers;  //!< Pointer for Handlers.
};
}
