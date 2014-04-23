#ifndef BS_CONNECTION_HPP
#define BS_CONNECTION_HPP




//Includes
#include <memory>   //std::shared_ptr
#include <utility>  //std::move




template< typename Signature >
class Signal;

template< typename Signature >
class Connection {
friend class Signal< Signature >;
private:
    typedef Signal< Signature > Signal_t;

    //Information shared between Connection objects
    //m_pcSignal is set to nullptr when it is destroyed
    struct Impl { Signal_t* m_pcSignal; };

    typedef std::shared_ptr< Impl > ImplPtr;
public:
    void disconnect();

    Connection( const Connection& cToCopy );
    Connection( Connection&& cToMove );
    Connection& operator =( const Connection& cToCopy );
    Connection& operator =( Connection&& cToMove );

    //Two connections are the same if they share the same Impl data.
    bool operator ==( Connection& cRight ) const;
    
private:
    //Only the signal the collection belongs to can construct or invalidate the connection.
    Connection( Signal_t* const pcSignal );
    void invalidate();

    ImplPtr m_pcImpl;
};

template< typename Signature >
void Connection< Signature >::disconnect() {
    Signal_t* pcSignal = m_pcImpl->m_pcSignal;
    if( pcSignal != nullptr )
        pcSignal->disconnect( *this );
}

template< typename Signature >
Connection< Signature >::Connection( const Connection& cToCopy ) :
    m_pcImpl( cToCopy.m_pcImpl ) {
}

template< typename Signature >
Connection< Signature >::Connection( Connection&& cToMove ) :
    m_pcImpl( std::move( cToMove.m_pcImpl ) ) {
}

template< typename Signature >
Connection< Signature >& Connection< Signature >::operator =( const Connection& cToCopy ) {
    m_pcImpl = cToCopy.m_pcImpl;
}

template< typename Signature >
Connection< Signature >& Connection< Signature >::operator =( Connection&& cToMove ) {
    m_pcImpl = std::move( cToMove.m_pcImpl );
}

template< typename Signature >
bool Connection< Signature >::operator ==( const Connection& cRight ) const {
    return m_pcImpl == cRight.m_pcImpl;
}

template< typename Signature >
Connection< Signature >::Connection( Signal_t* const pcSignal ) :
    m_pcImpl( new Impl( pcSignal ) ) {
}

template< typename Signature >
void Connection< Signature >::invalidate() {
    m_pcImpl->m_pcSignal = nullptr;
}



/*
Automatically disconnects the given connection when AutoConnection goes out of scope
*/
template< typename Signature >
class AutoConnection {
public:
    AutoConnection( const Connection< Signature >& cToCopy );
    AutoConnection( Connection< Signature >&& cToMove );
    ~AutoConnection();
private:
    Connection< Signature > m_cConnection;
};

template< typename Signature >
AutoConnection< Signature >::AutoConnection( const Connection< Signature >& cToCopy ) :
    m_cConnection( cToCopy ) {
}

template< typename Signature >
AutoConnection< Signature >::AutoConnection( Connection< Signature >&& cToMove ) :
    m_cConnection( std::move( cToMove ) ) {
}

template< typename Signature >
AutoConnection< Signature >::~AutoConnection() {
    m_cConnection.disconnect();
}


#endif //BS_CONNECTION_HPP