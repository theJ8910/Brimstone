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




namespace {
    using ::Brimstone::ushort;
    using ::Brimstone::uint;
    using ::Brimstone::ulong;
    using ::Brimstone::ulonglong;
    using ::Brimstone::int8;
    using ::Brimstone::uint8;
    using ::Brimstone::int16;
    using ::Brimstone::uint16;
    using ::Brimstone::int32;
    using ::Brimstone::uint32;
    using ::Brimstone::int64;
    using ::Brimstone::uint64;
    using ::Brimstone::intN;
    using ::Brimstone::uintN;
    using ::Brimstone::byte;
    using ::Brimstone::ubyte;
    using ::Brimstone::uchar;
    using ::Brimstone::ustring;
}

namespace UnitTest {

UT_TEST_BEGIN( ushort_type )
    return ( std::is_same< ushort, unsigned short >::value );
UT_TEST_END()

UT_TEST_BEGIN( uint_type )
    return ( std::is_same< uint, unsigned int >::value );
UT_TEST_END()

UT_TEST_BEGIN( ulong_type )
    return ( std::is_same< ulong, unsigned long >::value );
UT_TEST_END()

UT_TEST_BEGIN( ulonglong_type )
    return ( std::is_same< ulonglong, unsigned long long >::value );
UT_TEST_END()

UT_TEST_BEGIN( int8_size )
    return sizeof( int8 )   == 1;
UT_TEST_END()

UT_TEST_BEGIN( uint8_size )
    return sizeof( uint8 )  == 1;
UT_TEST_END()

UT_TEST_BEGIN( int16_size )
    return sizeof( int16 )  == 2;
UT_TEST_END()

UT_TEST_BEGIN( uint16_size )
    return sizeof( uint16 ) == 2;
UT_TEST_END()

UT_TEST_BEGIN( int32_size )
    return sizeof( int32 )  == 4;
UT_TEST_END()

UT_TEST_BEGIN( uint32_size )
    return sizeof( uint32 ) == 4;
UT_TEST_END()

UT_TEST_BEGIN( int64_size )
    return sizeof( int64 )  == 8;
UT_TEST_END()

UT_TEST_BEGIN( uint64_size )
    return sizeof( uint64 ) == 8;
UT_TEST_END()




#ifndef BS_BUILD_64BIT

UT_TEST_BEGIN( intN_size )
    return sizeof( intN )   == 4;
UT_TEST_END()

UT_TEST_BEGIN( uintN_size )
    return sizeof( uintN )  == 4;
UT_TEST_END()

#else //BS_BUILD_64BIT

UT_TEST_BEGIN( intN_size )
    return sizeof( intN )   == 8;
UT_TEST_END()

UT_TEST_BEGIN( uintN_size )
    return sizeof( uintN )  == 8;
UT_TEST_END()

#endif //BS_BUILD_64BIT




UT_TEST_BEGIN( byte_size )
    return sizeof( byte )  == 1;
UT_TEST_END()

UT_TEST_BEGIN( ubyte_size )
    return sizeof( ubyte ) == 1;
UT_TEST_END()

UT_TEST_BEGIN( uchar_size )
    return sizeof( uchar ) == 1;
UT_TEST_END()

UT_TEST_BEGIN( ustring_type )
    return ( std::is_same< uchar, ustring::value_type >::value );
UT_TEST_END()

}