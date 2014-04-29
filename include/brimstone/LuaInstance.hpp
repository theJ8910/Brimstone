/*
LuaInstance.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Represents a singular instance of Lua.
*/

#ifndef BS_LUAINSTANCE_HPP
#define BS_LUAINSTANCE_HPP




//Includes
#include <brimstone/types.hpp>          //uchar, int32
#include <brimstone/util/Sequence.hpp>  //Sequence
#include <brimstone/util/Misc.hpp>      //TupleSize
#include <tuple>                        //std::tuple




//Forward declarations
struct lua_State;




namespace Brimstone {

class LuaInstance {
friend class Lua;
public:
    LuaInstance();
    ~LuaInstance();

    void start();
    void stop();

    template< typename... Args >
    void callVoid( Args&&... xArgs );

    template< typename Return, typename... Args >
    void call( Return& cReturnOut, Args&&... xArgs );

    void load( const uchar* pszFilepath );

    void pushFunction( const uchar* pszFunctionName );

    //Push two or more values to the stack
    template< typename First, typename Second, typename... Remaining >
    void pushStack( First&& xFirst, Second&& xSecond, Remaining&&... xRemaining );

    template< typename... Types >
    void pushStack( std::tuple< Types... >& cTuple );

    //Push one value to the stack
    void pushStack( const bool b );
    void pushStack( const int32 i );
    void pushStack( const float f );
    void pushStack( const double f );
    void pushStack( const uchar* pszString );

    //Push zero values to the stack
    void pushStack();

    //Pop two or more values from the stack
    template< typename First, typename Second, typename... Remaining >
    void popStack( First& xFirstOut, Second& xSecondOut, Remaining&... xRemainingOut );

    template< typename... Types >
    void popStack( std::tuple< Types... >& cTuple );

    //Pop one value from the stack
    void popStack( bool& bOut );
    void popStack( int32& iOut );
    void popStack( float& fOut );
    void popStack( double& fOut );
    void popStack( const uchar*& pszStringOut );

    //Pop zero values from the stack
    void popStack();
private:
    void openLibraries();

    template< typename T, int... Integers >
    void pushStackTuple( T& cTuple, Sequence< Integers... > seq );

    template< typename T, int... Integers >
    void popStackTuple( T& cTuple, Sequence< Integers... > seq );

    void callFunction( const int32 iArgs, const int32 iReturnValues );

private:
    lua_State* m_pcState;
};

template< typename... Args >
void LuaInstance::callVoid( Args&&... xArgs ) {
    //Push arguments to the stack
    pushStack( std::forward< Args >( xArgs )... );

    //Call the function with the appropriate # of arguments and no return values
    callFunction( sizeof...( Args ), 0 );
}

template< typename Return, typename... Args >
void LuaInstance::call( Return& cReturnOut, Args&&... xArgs ) {
    //Push arguments to the stack
    pushStack( std::forward< Args >( xArgs )... );

    //Call the function with the appropriate # of arguments and return values
    callFunction( sizeof...( Args ), typename TupleSize< Return >::size() );

    //Pop the return values
    popStack( cReturnOut );
}

template< typename First, typename Second, typename... Remaining >
void LuaInstance::pushStack( First&& xFirst, Second&& xSecond, Remaining&&... xRemaining ) {
    //Push the first argument (calls the appropriate overload based on the type of First)
    pushStack( xFirst );

    //Push the second argument + any other arguments
    //If there are remaining arguments, this is a recursive call
    //Otherwise, this calls an overload like the above call
    pushStack( xSecond, xRemaining... );
}

template< typename... Types >
void LuaInstance::pushStack( std::tuple< Types... >& cTuple ) {
    pushStackTuple( cTuple, typename MakeSequence< sizeof...( Types ) >::type() );
}

template< typename First, typename Second, typename... Remaining >
void LuaInstance::popStack( First& xFirstOut, Second& xSecondOut, Remaining&... xRemainingOut ) {
    popStack( xFirstOut );
    popStack( xSecondOut, xRemainingOut... );
}

template< typename... Types >
void LuaInstance::popStack( std::tuple< Types... >& cTuple ) {
    //Pop values from the stack into the tuple.
    //This needs to be done in reverse order, since the return values are pushed
    //to the stack in a LIFO order.
    popStackTuple( cTuple, typename MakeReverseSequence< sizeof...( Types ) >::type() );
}

template< typename T, int... Integers >
void LuaInstance::pushStackTuple( T& cTuple, Sequence< Integers... > cSequence ) {
    //Call the multiple parameter pushStack, providing each of the tuples' elements as parameters
    pushStack( std::get< Integers >( cTuple )... );
}

template< typename T, int... Integers >
void LuaInstance::popStackTuple( T& cTuple, Sequence< Integers... > cSequence ) {
    //Call the multiple parameter popStack, providing each of the tuples' elements as output parameters
    popStack( std::get< Integers >( cTuple )... );
}

}




#endif //BS_LUAINSTANCE_HPP