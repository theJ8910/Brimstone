/*
util/Sequence.hpp
-----------------------
Copyright (c) 2014, theJ89

Code originally written by Johannes Schaub
http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer

Description:
    Sequences are template meta-programming helper objects.
    
    In general MakeSequence< N >::type (where N is some number greater than 0)
    will be a Sequence< 0, 1, 2, ..., N - 1 >.

    In general, MakeReverseSequence< N >::type (where N is some number greater than 0)
    will be a Sequence< N - 1, ..., 2, 1, 0 >.
    
    A variadic template function can be designed that takes a Sequence as a parameter like so:
        template< int... Integers >
        void someFunction( Sequence< Integers... > s ) {
            //Code utilizing "Integers" goes here
        }

    This function can then be invoked like so:
        someFunction( MakeSequence< 10 >::type() )

    Which is equivalent to:
        someFunction( Sequence< 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 >() )

    From this, we can see that someFunction will be called with an "Integers..." parameter pack of { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }.

    This parameter pack can be expanded and used in an expression you want to repeat.
    This is particularly useful for "unpacking" an std::tuple and passing its values to a function:
        anotherFunction( std::get< Integers >( tuple )... )

    
*/
#ifndef BS_UTIL_SEQUENCE_HPP
#define BS_UTIL_SEQUENCE_HPP




namespace Brimstone {

template< int... >
struct Sequence {};

template< int N, int... UpperIntegers >
struct MakeSequence : public MakeSequence< N - 1, N - 1, UpperIntegers... > {};

template< int... UpperIntegers >
struct MakeSequence< 0, UpperIntegers... > {
    typedef Sequence< UpperIntegers... > type;
};

template< int N, int... UpperIntegers >
struct MakeReverseSequence : public MakeReverseSequence< N - 1, UpperIntegers..., N - 1 > {};

template< int... UpperIntegers >
struct MakeReverseSequence< 0, UpperIntegers... > {
    typedef Sequence< UpperIntegers... > type;
};

}




#endif //BS_UTIL_SEQUENCE_HPP