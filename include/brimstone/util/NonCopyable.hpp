/*
util/NonCopyable.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    Declares a struct, NonCopyable, that classes can inherit from
    to prevent objects of that class from being copied.
*/
#ifndef BS_UTIL_NONCOPYABLE_HPP
#define BS_UTIL_NONCOPYABLE_HPP

namespace Brimstone {

struct NonCopyable {
    NonCopyable()                                 = default;
    NonCopyable( const NonCopyable& )             = delete;
    NonCopyable& operator =( const NonCopyable& ) = delete;
};

}

#endif //BS_UTIL_NONCOPYABLE_HPP
