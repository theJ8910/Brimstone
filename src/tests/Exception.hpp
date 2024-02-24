/*
Exception.hpp
-------------
Copyright (c) 2024, theJ89

Description:
    Exception classes used by UnitTests are defined here.
*/
#ifndef UT_EXCEPTION_HPP
#define UT_EXCEPTION_HPP




//Includes
#include <string>   //std::string




namespace UnitTest {

/*
Exception
---------

Description:
    Base class for exceptions.
*/
class Exception {
public:
    Exception();
    Exception( const char* description );
    Exception( const std::string& description );

    virtual const std::string getDescription() const;
private:
    std::string m_description;
};

/*
EOFError
--------

Description:
    Thrown when EOF (end-of-file) is encountered unexpectedly.
*/
class EOFError : public Exception { using Exception::Exception; };

}

#endif //UT_EXCEPTION_HPP
