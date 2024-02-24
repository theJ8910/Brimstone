/*
Exception.cpp
-------------
Copyright (c) 2024, theJ89

Description:
    See Exception.hpp for more information.
*/




//Includes
#include "Exception.hpp"    //Header file




namespace UnitTest {

/*
Exception::Exception{1}
-----------------------

Description:
    Exception constructor. Initializes the Exception with no description.

Arguments:
    N/A

Returns:
    N/A
*/
Exception::Exception() {
}

/*
Exception::Exception{2}
-----------------------

Description:
    Exception constructor. Initializes the Exception with the given description.

Arguments:
    description:    A description of the exception.

Returns:
    N/A
*/
Exception::Exception( const char* description )
    : m_description( description ) {
}

/*
Exception::Exception{3}
-----------------------

Description:
    Exception constructor. Initializes the Exception with the given description.

Arguments:
    description:    A description of the exception.

Returns:
    N/A
*/
Exception::Exception( const std::string& description )
    : m_description( description ) {
}

/*
Exception::getDescription
-------------------------

Description:
    Returns the exception's description.

Arguments:
    N/A

Returns:
    N/A
*/
const std::string Exception::getDescription() const {
    return m_description;
}

}
