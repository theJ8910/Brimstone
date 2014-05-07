/*
Logger.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines an interface for loggers, ILogger.

    Provides two logger implementations:
        ConsoleLogger (for logging to std::cout)
        FileLogger (for logging to a file)
    
    Provides a static Loggers class that loggers can be added or removed to.
    By doing Loggers::write( message, type ); you can write to all the registered loggers at once.

    Convenience methods are provided for writing messages of each type to registered loggers:
        logDetail
        logInfo
        logWarning
        logError
*/
#ifndef BS_LOGGER_HPP
#define BS_LOGGER_HPP




//Include
#include <fstream>              //std::ofstream
#include <vector>               //std::vector
#include <initializer_list>     //std::initializer_list
#include <brimstone/types.hpp>  //uchar




namespace Brimstone {

enum class LogMessageType {
    DETAIL, INFO, WARNING, ERR      //Has to be ERR because Windows has a macro for ERROR, ugh
};

const uchar* logMessageTypeToString( LogMessageType eType );

class ILogger {
public:
    virtual void write( const uchar* pszString, LogMessageType eType ) = 0;
};

class AbstractLogger : public ILogger {
public:
    AbstractLogger();
    void setFilter( std::initializer_list< LogMessageType > il );
    bool passesFilter( const LogMessageType eFilter ) const;
private:
    int32   m_eFilter;
};

class ConsoleLogger : public AbstractLogger {
public:
    virtual void write( const uchar* pszString, LogMessageType eType );
};

class FileLogger : public AbstractLogger {
public:
    FileLogger( const uchar* pszFilepath );
    virtual void write( const uchar* pszString, LogMessageType eType );
private:
    std::ofstream m_fout;
};

class Loggers {
public:
    static void write( const uchar* pszString, LogMessageType eType = LogMessageType::INFO );
    static void add( ILogger& cLogger );
    static void remove( ILogger& cLogger );
private:
    static std::vector< ILogger* > m_acLoggers;
};

//Convenience functions
inline void logDetail( const uchar* pszString ) {
    Loggers::write( pszString, LogMessageType::DETAIL );
}

inline void logInfo( const uchar* pszString ) {
    Loggers::write( pszString, LogMessageType::INFO );
}

inline void logWarning( const uchar* pszString ) {
    Loggers::write( pszString, LogMessageType::WARNING );
}

inline void logError( const uchar* pszString ) {
    Loggers::write( pszString, LogMessageType::ERR );
}

}




#endif //BS_LOGGER_HPP