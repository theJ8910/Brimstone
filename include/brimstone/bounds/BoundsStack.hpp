/*
bounds/BoundsStack.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    A specialized stack for storing bounds.

    In addition to functionality that generic stacks have,
    bounds stacks have a .pushIntersect( bounds ) method that
    pushes the intersect of the given bounds and the top bounds in the stack.
*/
#ifndef BS_BOUNDS_BOUNDSSTACK_HPP
#define BS_BOUNDS_BOUNDSSTACK_HPP




//Includes
#include <vector>   //std::vector




namespace Brimstone {

template< typename T >
class BoundsStack {
public:
    void        push( const T bounds );
    void        pushIntersect( const T bounds );
    void        pop();
    T&          top();

    void        clear();

    size_t      size() const;
    bool        empty() const;

    T&          operator []( const size_t index );
    const T&    operator []( const size_t index ) const;
private:
    std::vector< T > m_stack;
};

template< typename T >
void BoundsStack::push( const T bounds ) {
    m_stack.push_back( bounds );
}

template< typename T >
void BoundsStack::pushIntersect( const T bounds ) {
    m_stack.push_back( intersect( m_stack.back(), bounds ) );
}

template< typename T >
void BoundsStack::pop() {
    m_stack.pop_back();
}

template< typename T >
Bounds2i& BoundsStack::top() {
    return m_stack.back();
}

template< typename T >
void BoundsStack::clear() {
    m_stack.clear();
}

template< typename T >
size_t BoundsStack::size() const {
    return m_stack.size();
}

template< typename T >
bool BoundsStack<T>::empty() const {
    return m_stack.empty();
}

template< typename T >
T& BoundsStack<T>::operator []( const size_t index ) {
    BS_ASSERT_INDEX( index, m_stack.size() - 1 );

    return m_stack[index];
}

template< typename T >
const T& BoundsStack<T>::operator []( const size_t index ) const {
    BS_ASSERT_INDEX( index, m_stack.size() - 1 );

    return m_stack[index];
}

}

#endif //BS_BOUNDS_BOUNDSSTACK_HPP