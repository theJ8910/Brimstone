#ifndef BS_BUFFER_HPP
#define BS_BUFFER_HPP




template< typename Elem_t, typename Size_t >
class ArrayInputBuffer {
private:
    const Elem_t* const m_pa_ArrayIn;
    const Size_t        m__Size;
public:
    ArrayInputBuffer( const T* const pa_ArrayIn, const _Size );
    const Elem_t*   in() const;
    Size_t          size() const;
};

template< typename Elem_t, typename Size_t >
ArrayInputBuffer< Elem_t, Size_t >::ArrayInputBuffer( const T* const pa_ArrayIn, const _Size )
    : m_pa_ArrayIn( pa_ArrayIn ), m__Size( _Size ) {};

template< typename Elem_t, typename Size_t >
const Elem_t* ArrayInputBuffer< Elem_t, Size_t >::in() const {
    return m_paArrayIn;
};

template< typename Elem_t, typename Size_t >
Size_t ArrayInputBuffer< Elem_t, Size_t >::size() const {
    return m__Size;
};




template< typename String_t >
class StringInputBuffer {
private:
    const String_t& m_strIn;
    typedef typename String_t::value_type Elem_t;
    typedef typename String_t::size_type  Size_t;
public:
    StringInputBuffer( const String_t& strIn );
    const Elem_t* in() const;
    Size_t        size() const;
};

template< typename String_t >
StringInputBuffer< String_t >::StringInputBuffer( const String_t& strIn ) : m_strIn( strIn ) {};

template< typename String_t >
const StringInputBuffer< String_t >::Elem_t* StringInputBuffer< String_t >::in() const
{
    return m_strIn.c_str();
}

template< typename String_t >
StringInputBuffer< String_t >::Size_t StringInputBuffer< String_t >::size() const
{
    return m_strIn.size() + 1;
}




template< typename Elem_t, typename Size_t >
class ArrayOutputBuffer {
private:
    Elem_t* const   m_pa_ArrayOut;
    const Size_t    m__Size;
public:
    ArrayOutputBuffer( const T* const pa_ArrayOut, const _Size );
    Elem_t* out();
    Size_t size() const;
};

template< typename Elem_t, typename Size_t >
ArrayOutputBuffer< Elem_t, Size_t >::ArrayOutputBuffer( const T* const pa_ArrayOut, const _Size )
    : m_pa_ArrayOut( pa_ArrayOut ), m__Size( _Size ) {}

template< typename Elem_t, typename Size_t >
Elem_t* ArrayOutputBuffer< Elem_t, Size_t >::out() {
    return m_pa_ArrayOut;
}

template< typename Elem_t, typename Size_t >
Size_t ArrayOutputBuffer< Elem_t, Size_t >::size() const {
    return m__Size;
}




template< typename String_t >
class StringOutputBuffer {
private:
    String_t& m_strOut;
    typedef typename String_t::value_type Elem_t;
    typedef typename String_t::size_type  Size_t;
public:
    StringOutputBuffer( String_t& strOut );
    Elem_t* out();
    Size_t size() const;
};

template< typename String_t >
StringOutputBuffer< String_t >::StringOutputBuffer( String_t& strOut ) {
}

template< typename String_t >
StringOutputBuffer< String_t >::Elem_t* StringOutputBuffer< String_t >::out() {
    return &m_strOut[0];
}

template< typename String_t >
StringOutputBuffer< String_t >::Size_t StringOutputBuffer< String_t >::size() const {
    return m_strOut.size();
}




#endif //BS_BUFFER_HPP