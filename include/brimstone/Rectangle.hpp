
/*
Rectangle.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines a Rectangle class.
    Rectangles are defined by two x values (left, right) and two y values (top, bottom).

    In addition to methods to set/get these values directly, methods are provided to
    set/get the width/height of the Rectangle and set/get the "position" of the
    rectangle ( assumed to be the upper-left corner of the rectangle ).

    Rectangle is a template class that can take any numerical type.
    This file defines commonly used instantiations of Rectangle, namely:
        * LongRectangle: equivalent to Rectangle<long>
*/

#ifndef BS_RECTANGLE_HPP
#define BS_RECTANGLE_HPP




//Includes
#include "Exception.hpp"        




//Defines
//#define BS_RECTANGLE_ZERO




namespace Brimstone {

template< typename I >
class Rectangle {
public:
    I left, top, right, bottom;
public:
    //Construtors
    Rectangle();
    Rectangle( const I left, const I top, const I right, const I bottom );
    Rectangle( const I* const ltrb );

    //Rectangle represented as two x bounds and two y bounds
    void    set( const I left, const I top, const I right, const I bottom );
    void    get( I& left, I& top, I& right, I& bottom ) const;

    void    set( const I* const ltrb );
    void    get( I* const ltrbOut ) const;

    void    setLeft( const I left );
    I       getLeft() const;

    void    setTop( const I top );
    I       getTop() const;

    void    setRight( const I right );
    I       getRight() const;

    void    setBottom( const I bottom );
    I       getBottom() const;

    //Rectangle represented as width and height with top-left corner
    void    setDimensions( const I width, const I height );
    void    getDimensions( I& widthOut, I& heightOut );

    void    setWidth( const I width );
    I       getWidth() const;

    void    setHeight( const I height );
    I       getHeight() const;

    //Miscellaneous utility methods
    I       getArea() const;
    void    zero();
};

#ifdef BS_RECTANGLE_ZERO

template< typename I >
Rectangle<T>::Rectangle() : left( 0 ), top( 0 ), right( 0 ), bottom( 0 ) {
}

#else   //BS_RECTANGLE_ZERO

template< typename I >
Rectangle<I>::Rectangle() {
}

#endif  //BS_RECTANGLE_ZERO

template< typename I >
Rectangle<I>::Rectangle( const I left, const I top, const I right, const I bottom ) {
    set( left, top, right, bottom );
}

template< typename I >
Rectangle<I>::Rectangle( const I* const ltrb ) {
    set( ltrb );
}

template< typename I >
void Rectangle<I>::set( const I left, const I top, const I right, const I bottom ) {
    this->left      = left;
    this->top       = top;
    this->right     = right;
    this->bottom    = bottom;
}

template< typename I >
void Rectangle<I>::get( I& leftOut, I& topOut, I& rightOut, I& bottomOut ) const {
    leftOut     = left;
    topOut      = top;
    rightOut    = right;
    bottomOut   = bottom;
}

template< typename I >
void Rectangle<I>::set( const I* const ltrb ) {
    if( ltrb == nullptr )
        throw NullPointerException();

    left    = ltrb[ 0 ];
    top     = ltrb[ 1 ];
    right   = ltrb[ 2 ];
    bottom  = ltrb[ 3 ];
}

template< typename I >
void Rectangle<I>::get( I* const ltrbOut ) const {
    if( ltrbOut == nullptr )
        throw NullPointerException();

    ltrbOut[ 0 ] = left;
    ltrbOut[ 1 ] = top;
    ltrbOut[ 2 ] = right;
    ltrbOut[ 3 ] = bottom;
}

template< typename I >
void Rectangle<I>::setLeft( const I left ) {
    this->left = left;
}

template< typename I >
I Rectangle<I>::getLeft() const {
    return left;
}

template< typename I >
void Rectangle<I>::setTop( const I top ) {
    this->top = top;
}

template< typename I >
I Rectangle<I>::getTop() const {
    return top;
}

template< typename I >
void Rectangle<I>::setRight( const I right ) {
    this->right = right;
}

template< typename I >
I Rectangle<I>::getRight() const {
    return right;
}

template< typename I >
void Rectangle<I>::setBottom( const I bottom ) {
    this->bottom = bottom;
}

template< typename I >
I Rectangle<I>::getBottom() const {
    return bottom;
}

template< typename I >
void Rectangle<I>::setDimensions( const I width, const I height ) {
    right   = left  + width;
    bottom  = top   + height;
}

template< typename I >
void Rectangle<I>::getDimensions( I& widthOut, I& heightOut ) {
    widthOut   = right  - left;
    heightOut  = bottom - top;
}

template< typename I >
void Rectangle<I>::setWidth( const I width ) {
    right = left + width;
}

template< typename I >
I Rectangle<I>::getWidth() const {
    return right - left;
}

template< typename I >
void Rectangle<I>::setHeight( const I height ) {
    bottom = top + height;
}

template< typename I >
I Rectangle<I>::getHeight() const {
    return bottom - top;
}

template< typename I >
I Rectangle<I>::getArea() const {
    return getWidth() * getHeight();
}

template< typename I >
void Rectangle<I>::zero() {
    left    = 0;
    top     = 0;
    right   = 0;
    bottom  = 0;
}

typedef Rectangle< long > LongRectangle;

}




#endif //BS_RECTANGLE_HPP
