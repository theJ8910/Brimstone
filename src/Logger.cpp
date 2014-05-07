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

std::vector< ILogger* > Loggers::m_acLoggers;

const uchar* logMessageTypeToString( LogMessageType eType ) {
    static const uchar* apszLMTtoString[] = {
        "DETAIL", "INFO", "WARNING", "ERROR"
    };
    return apszLMTtoString[ (int32)eType ];
}

//0xFFFFFFFF = 1 for all bits
AbstractLogger::AbstractLogger() : m_eFilter( 0xFFFFFFFF ) {
}

void AbstractLogger::setFilter( std::initializer_list< LogMessageType > il ) {
    m_eFilter = 0;
    for( auto eType : il )
        m_eFilter |= ( 1 << (int32)eType );
}

bool AbstractLogger::passesFilter( const LogMessageType eType ) const {
    int32 iMask = ( 1 << (int32)eType );
    return ( iMask & m_eFilter ) == iMask;
}

void ConsoleLogger::write( const uchar* pszString, LogMessageType eType ) {
    if( !passesFilter( eType ) )
        return;

    //Error messages go to std::cerr, all other messages go to std::cout
    if( eType == LogMessageType::ERR )
        std::cerr << "[" << logMessageTypeToString( eType ) << "] " << pszString << std::endl;
    else
        std::cout << "[" << logMessageTypeToString( eType ) << "] " << pszString << std::endl;
}

FileLogger::FileLogger( const uchar* pszFilepath ) :
    m_fout( pszFilepath, std::ofstream::out | std::ofstream::app | std::ofstream::ate ) {

    if( m_fout.bad() )
        throw IOException();
}

void FileLogger::write( const uchar* pszString, LogMessageType eType ) {
    if( !passesFilter( eType ) )
        return;

    m_fout << "[" << logMessageTypeToString( eType ) << "] " << pszString << std::endl;
}

void Loggers::write( const uchar* pszString, LogMessageType eType ) {
    for( ILogger* p : m_acLoggers )
        p->write( pszString, eType );
}

void Loggers::add( ILogger& cLogger ) {
    m_acLoggers.push_back( &cLogger );
}

void Loggers::remove( ILogger& cLogger ) {
    auto it = std::find( m_acLoggers.begin(), m_acLoggers.end(), &cLogger );

    if( it == m_acLoggers.end() )
        throw NoSuchElementException();

    m_acLoggers.erase( it );
}

}
