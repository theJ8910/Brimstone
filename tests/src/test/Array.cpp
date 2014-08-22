/*
test/Array.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Array< T, N >
*/




//Includes
#include "../Test.hpp"              //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"             //allEqual, allEqualTo, copyAll

#include <brimstone/util/Array.hpp>




namespace {
    typedef ::Brimstone::Array< int, 5 > Array5i;

    const size_t cv_size         = 5;
    const int    cv_values[5]    { 1, 2, 3, 4,  5 };
    const int    cv_valuesAlt[5] { 6, 7, 8, 9, 10 };
}

namespace UnitTest {

UT_TEST_BEGIN( Array_constructorFill )
    Array5i o( 1 );

    return allEqualTo( o.m_data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Array_constructorCppRange )
    Array5i o( cv_values );

    return allEqual( o.m_data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Array_constructorInitializerList )
    Array5i o( { cv_values[0], cv_values[1], cv_values[2], cv_values[3], cv_values[4] } );

    return allEqual( o.m_data, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Array_setCppRange )
    Array5i o( cv_values );

    o.set( cv_valuesAlt );

    return allEqual( o.m_data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Array_setInitializerList )
    Array5i o( cv_values );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3], cv_valuesAlt[4] } );

    return allEqual( o.m_data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Array_getCppRange )
    int values[5];
    copyAll( cv_values, values );
    Array5i o( cv_valuesAlt );

    o.get( values );

    return allEqual( o.m_data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Array_fill )
    Array5i o( cv_values );

    o.fill( 1 );

    return allEqualTo( o.m_data, 1 );
UT_TEST_END()

UT_TEST_BEGIN( Array_begin )
    Array5i o( cv_values );

    return o.begin() == std::begin( o.m_data );
UT_TEST_END()

UT_TEST_BEGIN( Array_beginConst )
    const Array5i o( cv_values );

    return o.begin() == std::begin( o.m_data );
UT_TEST_END()

UT_TEST_BEGIN( Array_cbegin )
    const Array5i o( cv_values );

    return o.cbegin() == std::cbegin( o.m_data );
UT_TEST_END()

UT_TEST_BEGIN( Array_end )
    Array5i o( cv_values );

    return o.end() == std::end( o.m_data );
UT_TEST_END()

UT_TEST_BEGIN( Array_endConst )
    const Array5i o( cv_values );

    return o.end() == std::end( o.m_data );
UT_TEST_END()

UT_TEST_BEGIN( Array_cend )
    const Array5i o( cv_values );

    return o.cend() == std::cend( o.m_data );
UT_TEST_END()

UT_TEST_BEGIN( Array_rangedFor )
    Array5i o( cv_values );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( o.m_data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Array_rangedForConst )
    int data[cv_size];
    copyAll( cv_values, data );
    const Array5i o( cv_valuesAlt );

    int idx = 0;
    for( int i : o )
        data[idx++] = i;

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Array_index )
    Array5i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        o[i] = cv_valuesAlt[i];

    return allEqual( o.m_data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Array_indexConst )
    int data[cv_size];
    const Array5i o( cv_values );

    for( int i = 0; i < cv_size; ++i )
        data[i] = o[i];

    return allEqual( data, cv_values );
UT_TEST_END()

}