/*
Image.cpp
---------
Copyright (c) 2024, theJ89

Description:
    See Image.hpp for more information.
*/




//Includes
#include <brimstone/Image.hpp>  //Header file
#include <png.h>                //png_*




namespace {




//Constants
constexpr int IMAGE_PNG_SIG_SIZE = 8;




}




namespace Brimstone {




Image::Image() :
    m_data( nullptr ),
    m_size( 0, 0 ) {
}

Image::Image( ubyte* const data, const Size2i size ) :
    m_data( data ),
    m_size( size ) {
}

Image::Image( Image&& toMove ) :
    m_data( toMove.m_data ),
    m_size( toMove.m_size ) {

    toMove.m_data = nullptr;
}

Image& Image::operator =( Image&& toMove ) {
    m_data = toMove.m_data;
    m_size = toMove.m_size;
    toMove.m_data = nullptr;

    return *this;
}

Image::~Image() {
    destroy();
}

bool Image::isValid() const {
    return m_data != nullptr;
}

void Image::set( ubyte* const data, const Size2i size ) {
    //Destroy the previous image data if any was stored:
    destroy();

    //Set the new image data:
    m_data = data;
    m_size = size;
}

/*
Image::loadPNG
--------------

Description:
    Loads a PNG image (using libPNG) from the file at the given path, filename, into this Image.
    If the image was loaded successfully, true is returned.
    Otherwise, false is returned.

Arguments:
    filename:  The path to the PNG image to load.

Returns:
    bool:      true if the PNG image was loaded successfully, false otherwise.
*/
bool Image::loadPNG( const ustring& filename ) {
    //Destroy the previous image data if any was stored:
    destroy();

    //Try to open file
    FILE* file = fopen( filename.c_str(), "rb" );
    if( file == nullptr )
        return false;

    //Try to read the file's signature
    ubyte sig[IMAGE_PNG_SIG_SIZE];
    if( fread( sig, 1, IMAGE_PNG_SIG_SIZE, file ) != IMAGE_PNG_SIG_SIZE ) {
        fclose( file );
        return false;
    }

    //Confirm that signature is valid
    if( !png_check_sig( sig, IMAGE_PNG_SIG_SIZE ) ) {
        fclose( file );
        return false;
    }

    //Try to create read struct
    png_structp read = png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr );
    if( !read ) {
        fclose( file );
        return false;
    }

    //Try to create info struct
    png_infop info = png_create_info_struct( read );
    if( !info ) {
        png_destroy_read_struct( &read, nullptr, nullptr );
        fclose( file );
        return false;
    }

    //Error handling; the body of this if statement will be
    //jumped into if an error occurs while parsing the .png.
    ubyte* data = nullptr;
    ubyte** ptrs = nullptr;
    if( setjmp( png_jmpbuf( read ) ) ) {
        if( ptrs != nullptr )
            delete [] ptrs;
        if( data != nullptr )
            delete [] data;
        png_destroy_read_struct( &read, &info, nullptr );
        fclose( file );
        return false;
    }

    //Tell it to read from our file
    png_init_io( read, file );

    //We've already read the signature, so skip it
    png_set_sig_bytes( read, IMAGE_PNG_SIG_SIZE );

    //Read info about the image from the file;
    //this will allow us to call the png_get_* functions below
    png_read_info( read, info );

    png_uint_32 width    = png_get_image_width(  read, info );
    png_uint_32 height   = png_get_image_height( read, info );
    png_byte    depth    = png_get_bit_depth(    read, info );
    png_byte    color    = png_get_color_type(   read, info );

    //Perform conversions of native formats to RGBA
    switch( color ) {
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb( read );
        break;
    case PNG_COLOR_TYPE_GRAY:
        if( depth < 8 )
            png_set_expand_gray_1_2_4_to_8( read );
        depth = 8;
        break;
    }

    //If a transparency color is set, instead create an alpha channel derived from it
    if( png_get_valid( read, info, PNG_INFO_tRNS ) )
        png_set_tRNS_to_alpha( read );

    //PNG files support up to 16-bit precision (number of bits per channel).
    //We're expecting 8-bit precision, so we'll round down here if necessary.
    if( depth == 16 )
        png_set_strip_16( read );

    //Upgrade grayscale to RGB
    if( color == PNG_COLOR_TYPE_GRAY || color == PNG_COLOR_TYPE_GRAY_ALPHA )
        png_set_gray_to_rgb( read );

    //By this point the image should be guaranteed to be RGB8 or RGBA8.
    //If the image is RGB8, we need to convert it to RGBA8 by adding an alpha channel.
    //Because alpha is typically used to represent opacity, we want the alpha of our pixels to be 255 (full opacity).
    //Note: PNG_COLOR_TYPE_PALETTE and PNG_COLOR_TYPE_GRAY are here because they were upgraded to RGB earlier in the function.
    if( color == PNG_COLOR_TYPE_RGB || color == PNG_COLOR_TYPE_PALETTE || color == PNG_COLOR_TYPE_GRAY )
        png_set_filler( read, 0xFF, PNG_FILLER_AFTER );

    //Allocate a buffer large enough to hold the entirety of the image
    const size_t stride = 4*width;
    data = new ubyte[stride*height];

    //Allocate an array of row pointers.
    //Direct them to the start of individual rows in the buffer we allocated
    ptrs = new ubyte*[height];
    for( size_t i = 0; i < height; ++i )
        ptrs[i] = data + stride*i;

    //Finally, finally read the image
    png_read_update_info( read, info );
    //auto rowbytes = png_get_rowbytes( read, info );
    png_read_image( read, ptrs );

    //Delete the data we allocated & close the file
    delete [] ptrs;
    png_destroy_read_struct( &read, &info, nullptr );
    fclose( file );

    //Output the image
    m_data = data;
    m_size.set( width, height );

    return true;
}

void Image::destroy() {
    if( m_data != nullptr )
        delete m_data;
}

ubyte* Image::getData() const {
    return m_data;
}

Size2i Image::getSize() const {
    return m_size;
}




}
