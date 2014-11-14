/*
Exception.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Exception.hpp for more information.
*/




//Includes
#include <brimstone/Exception.hpp>
#include <brimstone/signals/Delegate.hpp>
#include <brimstone/Logger.hpp>
#include <boost/format.hpp>




//Macros
/*
BS_DEFINE_BASIC_EXCEPTION

Macro to automatically define a basic exception type, without a message
*/
#define BS_DEFINE_BASIC_EXCEPTION( exceptionName ) \
    ustring exceptionName::getDescription() const { \
        return #exceptionName; \
    }

/*
BS_DEFINE_MESSAGE_EXCEPTION

Macro to automatically define an exception type deriving from Exception.
Any exceptions using this macro take a message, which is returned when
getDescription() is called.
*/
#define BS_DEFINE_MESSAGE_EXCEPTION( exceptionName ) \
    exceptionName::exceptionName() { \
    } \
    exceptionName::exceptionName( const uchar* description ) : Exception( description ) { \
    } \
    exceptionName::exceptionName( const ustring& description ) : Exception( description ) { \
    }

namespace Brimstone {

Exception::Exception() {
}

Exception::Exception( const uchar* description )
    : m_description( description ) {
}

Exception::Exception( const ustring& description )
    : m_description( description ) {
}

ustring Exception::getDescription() const {
    return m_description;
};

BS_DEFINE_BASIC_EXCEPTION( DivideByZeroException );
BS_DEFINE_BASIC_EXCEPTION( DomainException );
BS_DEFINE_BASIC_EXCEPTION( NullPointerException );
BS_DEFINE_BASIC_EXCEPTION( SizeException );
BS_DEFINE_BASIC_EXCEPTION( BoundsException );
BS_DEFINE_BASIC_EXCEPTION( SingletonException );
BS_DEFINE_BASIC_EXCEPTION( NoSuchElementException );
BS_DEFINE_BASIC_EXCEPTION( UnexpectedResultException );
BS_DEFINE_BASIC_EXCEPTION( FormatException );
BS_DEFINE_BASIC_EXCEPTION( IOException );
BS_DEFINE_BASIC_EXCEPTION( NotImplementedException );
BS_DEFINE_BASIC_EXCEPTION( MalformedStringException );
BS_DEFINE_MESSAGE_EXCEPTION( GraphicsException );
BS_DEFINE_MESSAGE_EXCEPTION( LuaException );

namespace Private {
    void defaultUncaughtExceptionHandler( const IException& exception ) {
        logError( ( boost::format( "Uncaught exception: %s" ) % exception.getDescription() ).str() );
    }
    UncaughtExceptionHandler uncaughtExceptionHandler( defaultUncaughtExceptionHandler );
}

void uncaughtException( const IException& exception ) {
    Private::uncaughtExceptionHandler( exception );
}

void setUncaughtExceptionHandler( UncaughtExceptionHandler handler ) {
    Private::uncaughtExceptionHandler = handler;
}

UncaughtExceptionHandler getUncaughtExceptionHandler() {
    return Private::uncaughtExceptionHandler;
}

}




//Not needed outside of this file
#undef BS_DEFINE_BASIC_EXCEPTION
#undef BS_DEFINE_MESSAGE_EXCEPTION