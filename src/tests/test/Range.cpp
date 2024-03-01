/*
test/Range.cpp
--------------
Copyright (c) 2024, theJ89

Description:
    Unit tests for Range< T, N >
*/




//Includes
#include "../Test.hpp"              //UT_TEST_BEGIN, UT_TEST_END
#include "../utils.hpp"             //allEqual, allEqualTo, copyAll

#include <cstddef>                  //std::size_t

#include <brimstone/util/Range.hpp>




namespace {
    typedef ::Brimstone::Range< int*, const int* > RangeI;
    using   ::Brimstone::NullPointerException;

    const std::size_t cv_size = 5;
    const int cv_values[5]    = { 1, 2, 3, 4,  5 };
    const int cv_valuesAlt[5] = { 6, 7, 8, 9, 10 };
}

namespace UnitTest {

UT_TEST_BEGIN( Range_constructorCppRange )
    int data[ cv_size ];
    copyAll( cv_values, data );
    RangeI o( data );

    return o.begin() == std::begin( data ) &&
           o.end()   == std::end(   data );
UT_TEST_END()

UT_TEST_BEGIN( Range_constructorBeginEnd )
    int data[ cv_size ];
    copyAll( cv_values, data );
    RangeI o( std::begin( data ), std::end( data ) );

    return o.begin() == std::begin( data ) &&
           o.end()   == std::end(   data );
UT_TEST_END()

UT_TEST_BEGIN( Range_constructorBeginSize )
    int data[ cv_size ];
    copyAll( cv_values, data );
    RangeI o( data, cv_size );

    return o.begin() == std::begin( data ) &&
           o.end()   == std::end(   data );
UT_TEST_END()

UT_TEST_BEGIN( Range_setCppRange )
    int data[ cv_size ];
    copyAll( cv_values, data );
    RangeI o( data );

    o.set( cv_valuesAlt );

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Range_setInitializerList )
    int data[ cv_size ];
    copyAll( cv_values, data );
    RangeI o( data );

    o.set( { cv_valuesAlt[0], cv_valuesAlt[1], cv_valuesAlt[2], cv_valuesAlt[3], cv_valuesAlt[4] } );

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Range_get )
    int data1[ cv_size ];
    copyAll( cv_valuesAlt, data1 );
    RangeI o( data1 );
    int data2[ cv_size ];
    copyAll( cv_values, data2 );

    o.get( data2 );

    return allEqual( data2, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Range_begin )
    int data[ cv_size ];
    RangeI o( data );

    return o.begin() == std::begin( data );
UT_TEST_END()

UT_TEST_BEGIN( Range_beginConst )
    int data[ cv_size ];
    const RangeI o( data );

    return o.begin() == std::begin( data );
UT_TEST_END()

UT_TEST_BEGIN( Range_cbegin )
    int data[ cv_size ];
    RangeI o( data );

    return o.cbegin() == std::begin( data );
UT_TEST_END()

UT_TEST_BEGIN( Range_end )
    int data[ cv_size ];
    RangeI o( data );

    return o.end() == std::end( data );
UT_TEST_END()

UT_TEST_BEGIN( Range_endConst )
    int data[ cv_size ];
    const RangeI o( data );

    return o.end() == std::end( data );
UT_TEST_END()

UT_TEST_BEGIN( Range_cend )
    int data[ cv_size ];
    RangeI o( data );

    return o.cend() == std::end( data );
UT_TEST_END()

UT_TEST_BEGIN( Range_rangedFor )
    int data[ cv_size ];
    copyAll( cv_values, data );
    RangeI o( data );

    int idx = 0;
    for( int& i : o )
        i = cv_valuesAlt[idx++];

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Range_rangedForConst )
    int data1[cv_size];
    copyAll( cv_values, data1 );
    const RangeI o( data1 );
    int data2[cv_size];
    copyAll( cv_valuesAlt, data2 );

    int idx = 0;
    for( int i : o )
        data2[idx++] = i;

    return allEqual( data2, cv_values );
UT_TEST_END()

UT_TEST_BEGIN( Range_index )
    int data[ cv_size ];
    copyAll( cv_values, data );
    RangeI r( data );

    for( std::size_t i = 0; i < cv_size; ++i )
        r[i] = cv_valuesAlt[i];

    return allEqual( data, cv_valuesAlt );
UT_TEST_END()

UT_TEST_BEGIN( Range_indexConst )
    int data1[ cv_size ];
    copyAll( cv_values, data1 );
    const RangeI r( data1 );
    int data2[ cv_size ];
    copyAll( cv_valuesAlt, data2 );

    for( std::size_t i = 0; i < cv_size; ++i )
        data2[i] = r[i];

    return allEqual( data2, cv_values );
UT_TEST_END()




#ifdef BS_CHECK_NULLPTR

UT_TEST_BEGIN( Range_constructorBeginEnd_nullPointer )
    int i;
    try {
        RangeI r( nullptr, &i );
        return false;
    } catch( const NullPointerException& ) {}

    try {
        RangeI r( &i, nullptr );
        return false;
    } catch( const NullPointerException& ) {}

    return true;
UT_TEST_END()

UT_TEST_BEGIN( Range_constructorBeginSize_nullPointer )
    try {
        RangeI r( nullptr, cv_size );
    } catch( const NullPointerException& ) {
        return true;
    }
    return false;
UT_TEST_END()

#endif //BS_CHECK_NULLPTR




}
