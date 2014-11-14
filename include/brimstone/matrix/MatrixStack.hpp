/*
matrix/MatrixStack.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    A specialized stack for storing matrices.

    In addition to functionality that generic stacks have,
    matrix stacks have a .pushMultiply( matrix ) method that
    multiplies the given matrix against the top matrix in the stack
    and pushes the result to the stack.

    MatrixStack is a template class that takes the type of matrix you wish to use as a parameter,
    e.g. Matrix4x4f, Matrix3x3f, etc.
*/
#ifndef BS_MATRIX_MATRIXSTACK_HPP
#define BS_MATRIX_MATRIXSTACK_HPP




//Includes
#include <brimstone/util/Macros.hpp>    //BS_ASSERT_INDEX

#include <vector>                       //std::vector



namespace Brimstone {

template< typename T >
class MatrixStack {
public:
    void        push( const T matrix );
    void        pushMultiply( const T matrix );
    void        pushIdentity();
    void        pop();
    T&          top();

    void        clear();

    size_t      size() const;
    bool        empty() const;

    T&          operator []( const size_t index );
    const T&    operator []( const size_t index ) const;
private:
    std::vector< T >    m_stack;
};

template< typename T >
void MatrixStack< T >::push( const T matrix ) {
    m_stack.push_back( matrix );
}

template< typename T >
void MatrixStack<T>::pushMultiply( const T matrix ) {
    m_stack.push_back( matrix * m_stack.back() );
}

template< typename T >
void MatrixStack<T>::pushIdentity() {
    T id;
    id.identity();

    m_stack.push_back( id );
}

template< typename T >
void MatrixStack<T>::pop() {
    m_stack.pop_back();
}

template< typename T >
T& MatrixStack<T>::top() {
    return m_stack.back();
}

template< typename T >
void MatrixStack<T>::clear() {
    m_stack.clear();
}

template< typename T >
size_t MatrixStack<T>::size() const {
    return m_stack.size();
}

template< typename T >
bool MatrixStack<T>::empty() const {
    return m_stack.empty();
}

template< typename T >
T& MatrixStack<T>::operator []( const size_t index ) {
    BS_ASSERT_INDEX( index, m_stack.size() - 1 );

    return m_stack[index];
}

template< typename T >
const T& MatrixStack<T>::operator []( const size_t index ) const {
    BS_ASSERT_INDEX( index, m_stack.size() - 1 );

    return m_stack[index];
}

}

#endif //BS_MATRIX_MATRXISTACK_HPP