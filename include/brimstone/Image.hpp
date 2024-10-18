/*
Image.hpp
---------
Copyright (c) 2024, theJ89

Description:
    Simple class for storing a raw image.
*/
#ifndef BRIMSTONE_IMAGE_HPP
#define BRIMSTONE_IMAGE_HPP




//Includes
#include <brimstone/types.hpp>  //Brimstone::ustring, Brimstone::ubyte
#include <brimstone/Size.hpp>   //Brimstone::Size2i




namespace Brimstone {




class Image {
public:
    Image();
    Image( ubyte* const data, const Size2i size );
    Image( const Image& toCopy ) = delete;
    Image& operator =( const Image& toCopy ) = delete;
    Image( Image&& toMove );
    Image& operator =( Image&& toMove );
    ~Image();

    void   set( ubyte* const data, const Size2i size );
    bool   loadPNG( const ustring& filename );
    void   destroy();

    bool   isValid() const;
    ubyte* getData() const;
    Size2i getSize() const;
private:
    ubyte* m_data;
    Size2i m_size;
};



}

#endif //BRIMSTONE_IMAGE_HPP
