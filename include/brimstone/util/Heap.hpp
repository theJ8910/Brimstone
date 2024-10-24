/*
util/Heap.hpp
-------------
Copyright (c) 2024, theJ89

Description:
    Defines the Heap class, which implements a binary heap.

    A heap is a tree-like data structure where every node in the tree satisfies the heap property.
    There are two kinds of heaps as far as the heap property is concerned - min heaps and max heaps:
    * min heap: The node's parent's key must be less than or equal to the node's key.
    * max heap: The node's parent's key must be greater than or equal to the node's key.

    A binary heap is a heap that is also a binary tree; in other words, it's a heap where every node in the tree has between 0 and 2 child nodes.
*/
#ifndef BS_UTIL_HEAP_HPP
#define BS_UTIL_HEAP_HPP




//Includes
#include <cstddef>                  //std::size_t

#include <brimstone/Exception.hpp>  //Brimstone::NoSuchElementException




namespace Brimstone::Private {




//Heap Type for minheaps:
template< typename Key >
class MinHeapType {
public:
    static inline bool compare( const Key left, const Key right ) { return left < right; }
};

//Heap Type for maxheaps:
template< typename Key >
class MaxHeapType {
public:
    static inline bool compare( const Key left, const Key right ) { return left > right; }
};

//This helper class implements the default class passed to the NodeKey template parameter in Heap:
template< typename Node >
class DefaultHeapNodeKey {
public:
    static inline Node getKey( const Node node ) { return node; };
};




} //namespace Brimstone::Private




namespace Brimstone {




//NOTE:
//    If NodeKey is specified, it should be a class that implements a static method, getKey(), which takes a Node and returns the key (of type Key) that corresponds to that node.
//    getKey() has the following signature:
//          Key getKey( const Node node );
//    Key can be any type you want, provided you can use the the < and > operators with it.
template< template< typename > typename HeapType, typename Node, typename NodeKey = Private::DefaultHeapNodeKey< Node > >
class Heap {
private:
    using Key = decltype( NodeKey::getKey( Node() ) );
public:
    Heap();
    ~Heap();

    void push( const Node node );
    Node pop();
    Node& peek();
    const Node& peek() const;

    Node remove( const Node node );
    Node removeKey( const Key key );
    Node removeIndex( const std::size_t index );

    Node* begin();
    const Node* begin() const;
    const Node* cbegin() const;

    Node* end();
    const Node* end() const;
    const Node* cend() const;

    Node& operator []( const std::size_t index );
    const Node& operator []( const std::size_t index ) const;

    std::size_t size() const;
    bool empty() const;
private:
    void siftDown( const std::size_t index );
    void resize( const std::size_t capacity );
private:
    Node*       m_array;
    std::size_t m_size;
    std::size_t m_capacity;
private:
    /*
    Heap::compare
    -------------

    Description:
        Compares the two given keys, left and right, with an appropriate comparison function for this type of heap.

    Arguments:
        left:   The key that appears on the left-hand side of the comparison operator.
        right:  The key that appears on the right-hand side of the comparison operator.

    Returns:
        bool:  true if left should come before right, false otherwise.
    */
    static inline bool compare( const Key left, const Key right ) { return HeapType<Key>::compare( left, right ); }

    /*
    Heap::getKey
    ------------

    Description:
        Given a node, returns the key for that node.
        Calls the static getKey method provided by the class passed to the NodeKey template parameter.

    Arguments:
        node:  The node to whose key to get.

    Returns:
        Key:  The key that corresponds to the given node.
    */
    static inline Key getKey( const Node node ) { return NodeKey::getKey( node ); }
};

/*
Heap::Heap
----------

Description:
    Default constructor. Initializes an empty heap.

Arguments:
    N/A

Returns:
    N/A
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Heap< HeapType, Node, NodeKey >::Heap() :
    m_array( nullptr ),
    m_size( 0 ),
    m_capacity( 0 ) {
}

/*
Heap::~Heap
-----------

Description:
    Destructor. Deletes the heap's internal array if one was allocated.

Arguments:
    N/A

Returns:
    N/A
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Heap< HeapType, Node, NodeKey >::~Heap() {
    if( m_array != nullptr )
        delete [] m_array;
}

/*
Heap::push
----------

Description:
    Inserts the given node into the heap.

Arguments:
    node:  The node to insert into the heap.

Returns:
    N/A
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
void Heap< HeapType, Node, NodeKey >::push( const Node node ) {
    //Double the capacity of the heap if we're at full capacity:
    if( m_size == m_capacity )
        resize( m_capacity != 0 ? m_capacity <<= 1 : 1 ); //NOTE: ( x <<= 1 ) == ( x *= 2 )

    //We'll need to determine where in the heap we're inserting the new node.
    //We'll consider inserting it at the end of the array initially:
    std::size_t index = m_size;
    Key         key   = getKey( node );

    //Next, we'll sift up - this involves moving nodes that should come before the node we're inserting down a level in the hierarchy:
    std::size_t parentIndex;
    while( index > 0 ) {
        //Calculate the index of the parent node and get the key for the parent node.
        parentIndex = ( index - 1 ) >> 1; //NOTE: ( x >> 1 ) == ( x / 2 )

        //If the node we're inserting should come before the parent node, move the parent node down a level:
        if( compare( key, getKey( m_array[ parentIndex ] ) ) ) {
            //Move the parent node down a level in the tree:
            m_array[ index ] = std::move( m_array[ parentIndex ] );

            //Change the index we're considering inserting the node at
            index = parentIndex;
        //Otherwise, the node we're inserting should be inserted here.
        } else {
            break;
        }
    }

    //Finally, insert the new node into the internal array at the given position:
    m_array[ index ] = node;

    //Increase the count of how many nodes are stored in the heap:
    ++m_size;
}

/*
Heap::pop
---------

Description:
    Removes and returns the node at the top of the heap.
    For a maxheap, this will be the node that had the largest key.
    For a minheap, this will be the node that had the smallest key.

Arguments:
    N/A

Returns:
    Node:  The node that was at the top of the heap.

Throws:
    NoSuchElementException:  If the heap was empty.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node Heap< HeapType, Node, NodeKey >::pop() {
    if( m_size == 0 )
        throw NoSuchElementException();
    Node node = std::move( m_array[ 0 ] );

    //Reduce the count of how many nodes are stored in the heap:
    --m_size;

    //Sift down from the top of the heap:
    siftDown( 0 );

    //Halve the capacity of the heap if the heap is at half capacity or less:
    std::size_t capacity = m_capacity >> 1; //NOTE: ( x >> 1 ) == ( x / 2 )
    if( m_size <= capacity )
        resize( capacity );

    //Return the node that was at the top of the heap:
    return node;
}

/*
Heap::peek{1}
-------------

Description:
    Returns a reference to the node at the top of the heap without removing it from the heap.
    For a maxheap, this will be the node with the largest key.
    For a minheap, this will be the node with the smallest key.
    This override is for mutable Heaps.

Arguments:
    N/A

Returns:
    Node:  The node at the top of the heap.

Throws:
    NoSuchElementException:  If the heap was empty.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node& Heap< HeapType, Node, NodeKey >::peek() {
    if( m_size == 0 )
        throw NoSuchElementException();
    return m_array[ 0 ];
}

/*
Heap::peek{2}
-------------

Description:
    Returns a reference to the const node at the top of the heap without removing it from the heap.
    For a maxheap, this will be the node with the largest key.
    For a minheap, this will be the node with the smallest key.
    This override is for const Heaps.

Arguments:
    N/A

Returns:
    Node:  The node at the top of the heap.

Throws:
    NoSuchElementException:  If the heap was empty.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
const Node& Heap< HeapType, Node, NodeKey >::peek() const {
    if( m_size == 0 )
        throw NoSuchElementException();
    return m_array[ 0 ];
}

/*
Heap::remove
------------

Description:
    Removes the given node from the heap.
    Multiple nodes can potentially match the given node, in which case the first matching node encountered is removed.

Arguments:
    node:  The node to remove.

Returns:
    Node:  The removed node.

Throws:
    NoSuchElementException:  If the heap was empty or a matching element couldn't be found.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node Heap< HeapType, Node, NodeKey >::remove( const Node node ) {
    if( m_size == 0 )
        throw NoSuchElementException();
    //TODO:
    //    There's probably a faster way to search for the element to remove here using the element's key, but this naive search will do for now:
    for( std::size_t i = 0; i < m_size; ++i ) {
        if( m_array[i] == node ) {
            return removeIndex( i );
        }
    }
}

/*
Heap::removeKey
---------------

Description:
    Removes a node from the heap with the given key.
    Multiple nodes can potentially have the same key as the given node, in which case the first node with a matching key encountered is removed.

Arguments:
    node:  The node to remove.

Returns:
    Node:  The removed node.

Throws:
    NoSuchElementException:  If the heap was empty or an element with a matching key couldn't be found.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node Heap< HeapType, Node, NodeKey >::removeKey( const Key key ) {
    if( m_size == 0 )
        throw NoSuchElementException();
    //TODO:
    //    There's probably a faster way to search for the element to remove here using the element's key, but this naive search will do for now:
    for( std::size_t i = 0; i < m_size; ++i ) {
        if( getKey( m_array[i] ) == key ) {
            return removeIndex( i );
        }
    }
}

/*
Heap::removeIndex
-----------------

Description:
    Removes the node from the heap at the given index in the internal array.

Arguments:
    index:  The index of the node to remove.

Returns:
    Node:  The removed node.

Throws:
    NoSuchElementException:  If the given index was out of range.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node Heap< HeapType, Node, NodeKey >::removeIndex( const std::size_t index ) {
    if( index >= m_size )
        throw NoSuchElementException();

    //Save the node at this index:
    Node node = m_array[ index ];

    //Decrease the count of how many nodes are stored in the heap:
    --m_size;

    //Reorganize the heap:
    siftDown( index );

    //Halve the capacity of the heap if the heap is at half capacity or less:
    std::size_t capacity = m_capacity >> 1; //NOTE: ( x >> 1 ) == ( x / 2 )
    if( m_size <= capacity )
        resize( capacity );

    //Return the removed node:
    return node;
}

/*
Heap::begin{1}
--------------

Description:
    Returns a pointer to mutable Node pointing towards the first node in the heap's internal array.
    This override is for mutable Heaps.

Arguments:
    N/A

Returns:
    Node*:  Pointer to the first node in the heap.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node* Heap< HeapType, Node, NodeKey >::begin() {
    return m_array;
}

/*
Heap::begin{2}
--------------

Description:
    Returns a pointer to constant Node pointing towards the first node in the heap's internal array.
    This override is for const Heaps.

Arguments:
    N/A

Returns:
    const Node*:  Pointer to the first node in the heap.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
const Node* Heap< HeapType, Node, NodeKey >::begin() const {
    return m_array;
}

/*
Heap::cbegin
------------

Description:
    Returns a pointer to constant Node pointing towards the first node in the heap's internal array.

Arguments:
    N/A

Returns:
    Node*:  Pointer to the first node in the heap.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
const Node* Heap< HeapType, Node, NodeKey >::cbegin() const {
    return m_array;
}

/*
Heap::end{1}
------------

Description:
    Returns a pointer to mutable Node pointing towards a non-existent node following the last node in the heap's internal array.
    This override is for mutable Heaps.

Arguments:
    N/A

Returns:
    Node*:  Pointer to the first node in the heap.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node* Heap< HeapType, Node, NodeKey >::end() {
    return m_array + m_size;
}

/*
Heap::end{2}
------------

Description:
    Returns a pointer to const Node pointing towards a non-existent node following the last node in the heap's internal array.
    This override is for const Heaps.

Arguments:
    N/A

Returns:
    Node*:  Pointer to a non-existent node following the last node in the heap.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
const Node* Heap< HeapType, Node, NodeKey >::end() const {
    return m_array + m_size;
}

/*
Heap::cend
----------

Description:
    Returns a pointer to const Node pointing towards a non-existent node following the last node in the heap's internal array.

Arguments:
    N/A

Returns:
    Node*:  Pointer to a non-existent node following the last node in the heap.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
const Node* Heap< HeapType, Node, NodeKey >::cend() const {
    return m_array + m_size;
}

/*
Heap::operator []{1}
--------------------

Description:
    Overloaded array subscript operator.
    Allows for accessing a node at a given index in the heap.
    This override is for mutable Heaps.

    WARNING:
        This function returns a reference to a mutable Node. If you modify the node, you should be careful not to modify anything that affects the sorting key used by the heap, as the heap
        will not rebalance itself to account for these changes.

Arguments:
    index:  The index of the node to retrieve.

Returns:
    Node&:  Reference to the node at the given index.

Throws:
    NoSuchElementException:  If the given index is out of range.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
Node& Heap< HeapType, Node, NodeKey >::operator []( const std::size_t index ) {
    if( index >= m_size )
        throw NoSuchElementException();
    return m_array[ index ];
}

/*
Heap::operator []{2}
--------------------

Description:
    Overloaded array subscript operator.
    Allows for accessing a node at a given index in the heap.
    This override is for const Heaps.

Arguments:
    index:  The index of the node to retrieve.

Returns:
    const Node&:  Reference to the node at the given index.

Throws:
    NoSuchElementException:  If the given index is out of range.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
const Node& Heap< HeapType, Node, NodeKey >::operator []( const std::size_t index ) const {
    if( index >= m_size )
        throw NoSuchElementException();
    return m_array[ index ];
}

/*
Heap::size
----------

Description:
    Returns the size of the heap; i.e. how many nodes are stored in the heap.

Arguments:
    N/A

Returns:
    std::size_t:  The size of the heap.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
std::size_t Heap< HeapType, Node, NodeKey >::size() const {
    return m_size;
}

/*
Heap::empty
----------

Description:
    Returns whether or not the heap is empty; i.e. if the size of the heap is equal to 0.

Arguments:
    N/A

Returns:
    bool:  true if the heap is empty, false otherwise.
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
bool Heap< HeapType, Node, NodeKey >::empty() const {
    return m_size == 0;
}

/*
Heap::siftDown
--------------

Description:
    This function is called after a node has been removed at the given index to reorganize the heap.
    
    This works by moving the last node in the heap to the position in the internal array indicated by the given index, then determining which node is foremost - the node itself or one of its
    children. If one of the node's children is foremost, then the foremost child swaps positions with the node and the process is repeated until the node itself is foremost.

    NOTE: The actual implementation differs slightly from how I've described it here, but the results are the same.

Arguments:
    index:  The index to start sifting down from.

Returns:
    N/A
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
void Heap< HeapType, Node, NodeKey >::siftDown( std::size_t index ) {
    //Nothing to sift down if the heap is empty:
    if( m_size == 0 )
        return;

    //Get the key of the last node in the internal array:
    Key key = getKey( m_array[ m_size ] );

    //NOTE: The recursion here is implemented with a while loop instead.
    std::size_t leftIndex, rightIndex;
    Key         leftKey;
    while( true ) {
        leftIndex  = ( index << 1 ) + 1; //NOTE: ( x << 1 ) == ( 2 * x )

        //If the current node doesn't have a left child, then it has no children and we can exit the loop:
        if( leftIndex >= m_size )
            break;

        rightIndex = leftIndex + 1;
        //The current node has a right child as well:
        if( rightIndex < m_size ) {
            //If the left child should come before the last node...
            leftKey = getKey( m_array[ leftIndex ] );
            if( compare( leftKey, key ) ) {
                //...and the right child should come before the left child, then the right child becomes the foremost node:
                if( compare( getKey( m_array[ rightIndex ] ), leftKey ) ) {
                    m_array[ index ] = std::move( m_array[ rightIndex ] );
                    index = rightIndex;
                //Otherwise, the left child becomes the foremost node:
                } else {
                    m_array[ index ] = std::move( m_array[ leftIndex ] );
                    index = leftIndex;
                }
            //Otherwise, if the right child should come before the last node, it becomes the foremost node:
            } else if( compare( getKey( m_array[ rightIndex ] ), key ) ) {
                m_array[ index ] = std::move( m_array[ rightIndex ] );
                index = rightIndex;
            //If neither the left nor right child should come before the last node, we're done - exit the loop:
            } else {
                break;
            }
        //The current node only has a left child:
        } else {
            //If the current node's left child should come before the last node, then move it to the current index:
            if( compare( getKey( m_array[ leftIndex ] ), key ) ) {
                m_array[ index ] = std::move( m_array[ leftIndex ] );
                index = leftIndex;
            //Otherwise, we're done - exit the loop:
            } else {
                break;
            }
        }
    }
    //Move the last node into the empty spot we opened up:
    m_array[ index ] = std::move( m_array[ m_size ] );
}

/*
Heap::resize
------------

Description:
    Resizes the internal array to the given capacity.
    m_size is expected to be less than or equal to the given capacity.

Arguments:
    capacity:  The size to resize the internal array to.

Returns:
    N/A
*/
template< template< typename > typename HeapType, typename Node, typename NodeKey >
void Heap< HeapType, Node, NodeKey >::resize( const std::size_t capacity ) {
    //Set the capacity of the heap:
    m_capacity = capacity;

    //Temporarily save old array:
    Node* oldArray = m_array;

    //If capacity isn't 0, create a new array, otherwise set the array to nullptr:
    m_array = capacity != 0 ? new Node[ capacity ] : nullptr;

    //If the old array existed:
    if( oldArray != nullptr ) {
        //Move entries to new array, if it exists:
        if( m_array != nullptr ) {
            for( std::size_t i = 0; i < m_size; ++i ) {
                m_array[i] = std::move( oldArray[i] );
            }
        }
        //Delete old array:
        delete [] oldArray;
    }
}




//Types
template< typename Node, typename NodeKey = Private::DefaultHeapNodeKey< Node > >
using MinHeap = Heap< Private::MinHeapType, Node, NodeKey >;

template< typename Node, typename NodeKey = Private::DefaultHeapNodeKey< Node > >
using MaxHeap = Heap< Private::MaxHeapType, Node, NodeKey >;




} //namespace Brimstone




#endif //BS_UTIL_HEAP_HPP
