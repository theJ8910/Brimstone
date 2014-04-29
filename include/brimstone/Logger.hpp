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
    By doing Loggers::write( message ); you can write to all the registered loggers at once.
*/
#ifndef BS_LOGGER_HPP
#define BS_LOGGER_HPP




//Include
#include <fstream>              //std::ofstream
#include <vector>               //std::vector
#include <brimstone/types.hpp>  //uchar




namespace Brimstone {

enum class LogMessageType {
    DETAIL, INFO, WARNING, ERR       //Has to be ERR because Windows has a macro for ERROR, ugh
};

const uchar* logMessageTypeToString( LogMessageType eType );

class ILogger {
public:
    virtual void write( const uchar* pszString, LogMessageType eType ) = 0;
};

class ConsoleLogger : public ILogger {
public:
    virtual void write( const uchar* pszString, LogMessageType eType );
};

class FileLogger : public ILogger {
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

}




#endif //BS_LOGGER_HPP