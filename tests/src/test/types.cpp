/*
test/types.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Unit tests for Brimstone's types
*/

//Includes
#include <type_traits>

#include "../Test.hpp"

#include <brimstone/types.hpp>

namespace Brimstone {
namespace UnitTest {

BS_UT_TEST_BEGIN( ushort_type )
    return ( std::is_same< ushort, unsigned short >::value );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uint_type )
    return ( std::is_same< uint, unsigned int >::value );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( ulong_type )
    return ( std::is_same< ulong, unsigned long >::value );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( ulonglong_type )
    return ( std::is_same< ulonglong, unsigned long long >::value );
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( int8_size )
    return sizeof( int8 )   == 1;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uint8_size )
    return sizeof( uint8 )  == 1;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( int16_size )
    return sizeof( int16 )  == 2;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uint16_size )
    return sizeof( uint16 ) == 2;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( int32_size )
    return sizeof( int32 )  == 4;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uint32_size )
    return sizeof( uint32 ) == 4;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( int64_size )
    return sizeof( int64 )  == 8;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uint64_size )
    return sizeof( uint64 ) == 8;
BS_UT_TEST_END()

#ifndef BS_BUILD_64BIT

BS_UT_TEST_BEGIN( intN_size )
    return sizeof( intN )   == 4;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uintN_size )
    return sizeof( uintN )  == 4;
BS_UT_TEST_END()

#else

BS_UT_TEST_BEGIN( intN_size )
    return sizeof( intN )   == 8;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uintN_size )
    return sizeof( uintN )  == 8;
BS_UT_TEST_END()

#endif //BS_BUILD_64BIT

BS_UT_TEST_BEGIN( byte_size )
    return sizeof( byte )  == 1;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( ubyte_size )
    return sizeof( ubyte ) == 1;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( uchar_size )
    return sizeof( uchar ) == 1;
BS_UT_TEST_END()

BS_UT_TEST_BEGIN( ustring_type )
    return ( std::is_same< uchar, ustring::value_type >::value );
BS_UT_TEST_END()

}
}