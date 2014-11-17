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

std::mutex                          Loggers::m_loggersMutex;
std::vector< Loggers::LoggerPair >  Loggers::m_loggers;
size_t                              Loggers::m_nextLoggerID = (size_t)-1;

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

void Loggers::write( const ustring& str, LogMessageType type ) {
    std::lock_guard< std::mutex > l( m_loggersMutex );
    for( LoggerPair& pair : m_loggers )
        pair.first->write( str.c_str(), type );
}

size_t Loggers::add( std::unique_ptr< ILogger >&& logger ) {
    std::lock_guard< std::mutex > l( m_loggersMutex );
    m_loggers.push_back( LoggerPair( std::move( logger ), ++m_nextLoggerID ) );

    return m_nextLoggerID;
}

void Loggers::remove( const size_t id ) {
    std::lock_guard< std::mutex > l( m_loggersMutex );

    for( auto it = std::begin( m_loggers ); it != std::end( m_loggers ); ++it ) {
        if( it->second == id ) {
            m_loggers.erase( it );
            return;
        }
    }

    throw NoSuchElementException();
}

}
