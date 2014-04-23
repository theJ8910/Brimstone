
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
    Rectangle( const I iLeft, const I iTop, const I iRight, const I iBottom );
    Rectangle( const I* const paiLTRB );

    //Rectangle represented as two x bounds and two y bounds
    void    set( const I iLeft, const I iTop, const I iRight, const I iBottom );
    void    get( I& iLeft, I& iTop, I& iRight, I& iBottom ) const;

    void    set( const I* const paiLTRB );
    void    get( I* const paiLTRBOut ) const;

    void    setLeft( const I iLeft );
    I       getLeft() const;

    void    setTop( const I iTop );
    I       getTop() const;

    void    setRight( const I iRight );
    I       getRight() const;

    void    setBottom( const I iBottom );
    I       getBottom() const;

    //Rectangle represented as width and height with top-left corner
    void    setDimensions( const I iWidth, const I iHeight );
    void    getDimensions( I& iWidthOut, I& iHeightOut );

    void    setWidth( const I iWidth );
    I       getWidth() const;

    void    setHeight( const I iHeight );
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
Rectangle<I>::Rectangle( const I iLeft, const I iTop, const I iRight, const I iBottom ) {
    set( iLeft, iTop, iRight, iBottom );
}

template< typename I >
Rectangle<I>::Rectangle( const I* const paiLTRB ) {
    set( paiLTRB );
}

template< typename I >
void Rectangle<I>::set( const I iLeft, const I iTop, const I iRight, const I iBottom ) {
    left    = iLeft;
    top     = iTop;
    right   = iRight;
    bottom  = iBottom;
}

template< typename I >
void Rectangle<I>::get( I& iLeftOut, I& iTopOut, I& iRightOut, I& iBottomOut ) const {
    iLeftOut    = left;
    iTopOut     = top;
    iRightOut   = right;
    iBottomOut  = bottom;
}

template< typename I >
void Rectangle<I>::set( const I* const paiLTRB ) {
    if( paiLTRB == NULL )
        throw NullPointerException();

    left    = paiLTRB[ 0 ];
    top     = paiLTRB[ 1 ];
    right   = paiLTRB[ 2 ];
    bottom  = paiLTRB[ 3 ];
}

template< typename I >
void Rectangle<I>::get( I* const paiLTRBOut ) const {
	if( paiLTRB == NULL )
        throw NullPointerException();

	paiLTRBOut[ 0 ] = left;
    paiLTRBOut[ 1 ] = top;
    paiLTRBOut[ 2 ] = right;
    paiLTRBOut[ 3 ] = bottom;
}

template< typename I >
void Rectangle<I>::setLeft( const I iLeft ) {
    left = iLeft;
}

template< typename I >
I Rectangle<I>::getLeft() const {
    return left;
}

template< typename I >
void Rectangle<I>::setTop( const I iTop ) {
    top = iTop;
}

template< typename I >
I Rectangle<I>::getTop() const {
    return top;
}

template< typename I >
void Rectangle<I>::setRight( const I iRight ) {
    right = iRight;
}

template< typename I >
I Rectangle<I>::getRight() const {
    return right;
}

template< typename I >
void Rectangle<I>::setBottom( const I iBottom ) {
    bottom = iBottom;
}

template< typename I >
I Rectangle<I>::getBottom() const {
    return bottom;
}

template< typename I >
void Rectangle<I>::setDimensions( const I iWidth, const I iHeight ) {
    right   = left  + iWidth;
    bottom  = top   + iHeight;
}

template< typename I >
void Rectangle<I>::getDimensions( I& iWidthOut, I& iHeightOut ) {
    iWidthOut   = right - left;
    iHeightOut  = bottom - top;
}

template< typename I >
void Rectangle<I>::setWidth( const I iWidth ) {
    right = left + iWidth;
}

template< typename I >
I Rectangle<I>::getWidth() const {
    return right - left;
}

template< typename I >
void Rectangle<I>::setHeight( const I iHeight ) {
    bottom = top + iHeight;
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