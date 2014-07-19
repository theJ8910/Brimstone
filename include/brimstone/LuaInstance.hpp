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
    void callVoid( Args&&... args );

    template< typename Return, typename... Args >
    void call( Return& returnOut, Args&&... args );

    void load( const uchar* filepath );

    void pushFunction( const uchar* functionName );

    //Push two or more values to the stack
    template< typename First, typename Second, typename... Remaining >
    void pushStack( First&& first, Second&& second, Remaining&&... remaining );

    template< typename... Types >
    void pushStack( std::tuple< Types... >& argTuple );

    //Push one value to the stack
    void pushStack( const bool value );
    void pushStack( const intN value );
    void pushStack( const float value );
    void pushStack( const double value );
    void pushStack( const uchar* value );

    //Push zero values to the stack
    void pushStack();

    //Pop two or more values from the stack
    template< typename First, typename Second, typename... Remaining >
    void popStack( First& firstOut, Second& secondOut, Remaining&... remainingOut );

    template< typename... Types >
    void popStack( std::tuple< Types... >& tupleOut );

    //Pop one value from the stack
    void popStack( bool& valueOut );
    void popStack( intN& valueOut );
    void popStack( float& valueOut );
    void popStack( double& valueOut );
    void popStack( const uchar*& valueOut );

    //Pop zero values from the stack
    void popStack();
private:
    void openLibraries();

    template< typename T, int... Integers >
    void pushStackTuple( T& tuple, Sequence< Integers... > seq );

    template< typename T, int... Integers >
    void popStackTuple( T& tuple, Sequence< Integers... > seq );

    void callFunction( const int32 args, const int32 returnValues );

private:
    lua_State* m_state;
};

template< typename... Args >
void LuaInstance::callVoid( Args&&... args ) {
    //Push arguments to the stack
    pushStack( std::forward< Args >( args )... );

    //Call the function with the appropriate # of arguments and no return values
    callFunction( sizeof...( Args ), 0 );
}

template< typename Return, typename... Args >
void LuaInstance::call( Return& returnOut, Args&&... args ) {
    //Push arguments to the stack
    pushStack( std::forward< Args >( args )... );

    //Call the function with the appropriate # of arguments and return values
    callFunction( sizeof...( Args ), typename TupleSize< Return >::size() );

    //Pop the return values
    popStack( returnOut );
}

template< typename First, typename Second, typename... Remaining >
void LuaInstance::pushStack( First&& first, Second&& second, Remaining&&... remaining ) {
    //Push the first argument (calls the appropriate overload based on the type of First)
    pushStack( first );

    //Push the second argument + any other arguments
    //If there are remaining arguments, this is a recursive call
    //Otherwise, this calls an overload like the above call
    pushStack( second, remaining... );
}

template< typename... Types >
void LuaInstance::pushStack( std::tuple< Types... >& argTuple ) {
    pushStackTuple( argTuple, typename MakeSequence< sizeof...( Types ) >::type() );
}

template< typename First, typename Second, typename... Remaining >
void LuaInstance::popStack( First& firstOut, Second& secondOut, Remaining&... remainingOut ) {
    popStack( firstOut );
    popStack( secondOut, remainingOut... );
}

template< typename... Types >
void LuaInstance::popStack( std::tuple< Types... >& tupleOut ) {
    //Pop values from the stack into the tuple.
    //This needs to be done in reverse order, since the return values are pushed
    //to the stack in a LIFO order.
    popStackTuple( tupleOut, typename MakeReverseSequence< sizeof...( Types ) >::type() );
}

template< typename T, int... Integers >
void LuaInstance::pushStackTuple( T& tuple, Sequence< Integers... > sequence ) {
    //Call the multiple parameter pushStack, providing each of the tuples' elements as parameters
    pushStack( std::get< Integers >( tuple )... );
}

template< typename T, int... Integers >
void LuaInstance::popStackTuple( T& tuple, Sequence< Integers... > sequence ) {
    //Call the multiple parameter popStack, providing each of the tuples' elements as output parameters
    popStack( std::get< Integers >( tuple )... );
}

}




#endif //BS_LUAINSTANCE_HPP