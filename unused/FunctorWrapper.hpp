#ifndef BS_FUNCTORWRAPPER_HPP
#define BS_FUNCTORWRAPPER_HPP




namespace Brimstone {

//This would otherwise be a pure interface, but the virtual destructor needs to be defined
class AbstractFunctorWrapper {
public:
    virtual ~AbstractFunctorWrapper() {};
    virtual void* getFunctorPointer() = 0;
    virtual AbstractFunctorWrapper* clone() = 0;
};

template< typename Functor_t >
class FunctorWrapper : public AbstractFunctorWrapper {
public:
    FunctorWrapper( const Functor_t& fnFunctor );
    FunctorWrapper( Functor_t&& fnFunctor );
    virtual ~FunctorWrapper();
    virtual void* getFunctorPointer();
    virtual AbstractFunctorWrapper* clone();
private:
    Functor_t m_fnFunctor;
};

template< typename Functor_t >
FunctorWrapper< Functor_t >::FunctorWrapper( const Functor_t& fnFunctor ) : m_fnFunctor( fnFunctor ) {}
template< typename Functor_t >
FunctorWrapper< Functor_t >::FunctorWrapper( Functor_t&& fnFunctor ) : m_fnFunctor( std::move( fnFunctor ) ) {}
template< typename Functor_t >
FunctorWrapper< Functor_t >::~FunctorWrapper() {}
template< typename Functor_t >
void* FunctorWrapper< Functor_t >::getFunctorPointer() { return reinterpret_cast< void* >( &m_fnFunctor ); }
template< typename Functor_t >
AbstractFunctorWrapper* FunctorWrapper< Functor_t >::clone() { return new FunctorWrapper( *this ); }

}




#endif //BS_FUNCTORWRAPPER_HPP