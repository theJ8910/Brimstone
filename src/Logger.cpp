/*
Logger.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Logger.hpp for more details.
*/




//Includes
#include <brimstone/Logger.hpp>     //Class header
#include <brimstone/Exception.hpp>  //NoSuchElementException
#include <iostream>                 //std::cout, std::cerr
#include <algorithm>                //std::find


namespace Brimstone {

std::vector< ILogger* > Loggers::m_loggers;

const uchar* logMessageTypeToString( LogMessageType type ) {
    static const uchar* lmtToString[] = {
        "DETAIL", "INFO", "WARNING", "ERROR"
    };
    return lmtToString[ (int32)type ];
}

//0xFFFFFFFF = 1 for all bits
AbstractLogger::AbstractLogger() : m_filter( 0xFFFFFFFF ) {
}

void AbstractLogger::setFilter( std::initializer_list< LogMessageType > il ) {
    m_filter = 0;
    for( auto eType : il )
        m_filter |= ( 1 << (int32)eType );
}

bool AbstractLogger::passesFilter( const LogMessageType type ) const {
    int32 mask = ( 1 << (int32)type );
    return ( mask & m_filter ) == mask;
}

void ConsoleLogger::write( const uchar* str, LogMessageType type ) {
    if( !passesFilter( type ) )
        return;

    //Error messages go to std::cerr, all other messages go to std::cout
    if( type == LogMessageType::ERR )
        std::cerr << "[" << logMessageTypeToString( type ) << "] " << str << std::endl;
    else
        std::cout << "[" << logMessageTypeToString( type ) << "] " << str << std::endl;
}

FileLogger::FileLogger( const uchar* filepath ) :
    m_fout( filepath, std::ios::out | std::ios::app | std::ios::ate ) {

    if( m_fout.bad() )
        throw IOException();
}

void FileLogger::write( const uchar* pszString, LogMessageType eType ) {
    if( !passesFilter( eType ) )
        return;

    m_fout << "[" << logMessageTypeToString( eType ) << "] " << pszString << std::endl;
}

void Loggers::write( const uchar* str, LogMessageType type ) {
    for( ILogger* p : m_loggers )
        p->write( str, type );
}

void Loggers::add( ILogger& logger ) {
    m_loggers.push_back( &logger );
}

void Loggers::remove( ILogger& logger ) {
    auto it = std::find( m_loggers.begin(), m_loggers.end(), &logger );

    if( it == m_loggers.end() )
        throw NoSuchElementException();

    m_loggers.erase( it );
}

}
