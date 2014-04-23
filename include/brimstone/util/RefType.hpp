/*
RefType.hpp
-----------------------

Description:
    Utility for template classes that allows them to automatically determine
    whether something should be passed by value or passed by reference.

    RefType<T> consists of two typedefs:  RefType<T>::ref, and  RefType<T>::const_ref.
    If T is an object:
        ref is:       T&
        const_ref is: const T&
    If T is a primitive type:
        ref is:       T
        const_ref is: const T

    Sample usage:
    #include "util/RefType.hpp"

    template< typename T>
    class SomeClass {
    private:
        typedef typename RefType<T>::ref       TRef_t;
        typedef typename RefType<T>::const_ref TConstRef_t;
    public:
        void SomeMethod( TConstRef_t param ) {
        };
    };
*/
#ifndef BS_REFTYPE_HPP
#define BS_REFTYPE_HPP




//Includes
#include <stdint.h>




namespace Brimstone {

template< typename T >
class RefType {
public:
    typedef T	     ref;
    typedef const T& const_ref;
};

#define MAKE_PRIMITIVE_REFTYPE_SPEC( type ) \
    template<>\
    class RefType< type > {\
    public:\
        typedef type       ref;\
        typedef const type const_ref;\
    };

MAKE_PRIMITIVE_REFTYPE_SPEC( bool     );
MAKE_PRIMITIVE_REFTYPE_SPEC( int8_t   );
MAKE_PRIMITIVE_REFTYPE_SPEC( uint8_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( int16_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( uint16_t );
MAKE_PRIMITIVE_REFTYPE_SPEC( int32_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( uint32_t );
MAKE_PRIMITIVE_REFTYPE_SPEC( int64_t  );
MAKE_PRIMITIVE_REFTYPE_SPEC( uint64_t );
MAKE_PRIMITIVE_REFTYPE_SPEC( float    );
MAKE_PRIMITIVE_REFTYPE_SPEC( double   );

//This macro is no longer necessary after this point
#undef MAKE_PRIMITIVE_REFTYPE_SPEC

}




#endif //BS_REFTYPE_HPP