#ifndef UTILS_VECTORUTILS_HPP_
#define UTILS_VECTORUTILS_HPP_
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  Created by Arthur Moncorge on Dec 22, 2009.
///  Copyright 2009. All rights reserved.
///
///  Internal class for vectors: this class is going to replace mathspackage/vecutils.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <iostream>
#include "macro.h"
#include "poolalloc.h"


template <class T>
class Vector : public std::vector<T, PoolAllocator<T> >
{
public:
    Vector();
    Vector(int n);                             // Allocate a std::vector of n
    Vector(int n, const T& a);                 // Allocate a std::vector of n and initialize to a
    inline int size() const;                   // Return size and cast to int (no warning in Windows)
    inline void push_front(const T& a);        // Push at front an element
    inline void push_back(const T& a);         // Push at end an element
    inline void push_back(const Vector<T>& a); // Push at end a std::vector
    inline int find(const T& a) const;         // Find element a
    inline void reallocate(int size);          // Resize std::vector to a longer std::vector and keep previous data
    inline void reallocate(int size, const T& a);// Resize std::vector to a longer std::vector and keep previous data
    // Additional methods available in Vector<T>
    // inline T& operator[](const int i);      //i'th element
    // void assign(int n, const T &a)          // Reallocate the std::vector of n and initialize to a
    // void clear();                           // Deallocate data
    
    
    
};


template <class T> void ReleaseVectorMemory(Vector<T> & v)
{
    //first destroy all objects in std::vector:
    v.clear();
    
    //now swap storage with an empty std::vector
    Vector<T> empty;
    
    v.swap(empty);
    
    //now empty holds the storage of v and v holds the storage of empty(which has now storage)
    
    //empty is destroyed when we exit this function so the memory is actually released
    
}

template <class T>
Vector<T>::Vector()
{
}

template <class T>
Vector<T>::Vector(int n)
{
    size_t nn = n;
    this->resize(nn);
}

template <class T>
Vector<T>::Vector(int n, const T& a)
{
    size_t nn = n;
    this->assign(nn,a);
}

// Cast size_t to int to avoid warnings in Windows
template <class T>
inline int Vector<T>::size() const
{
    size_t size = std::vector<T, PoolAllocator<T> >::size();
    return BEST_STATIC_CPPCAST(int, size);
}

// Extend Vector class with push_front method
template <class T>
inline void Vector<T>::push_front(const T& a)
{
    int nn = size();
    this->insert( this->begin(), a);
    int nn2 = size();
    if (nn2!=nn+1)
    {
        BEST_ERROR("push_front");
    }
}

// Extend with push_front for element
template <class T>
inline void Vector<T>::push_back(const T& a)
{
    this->std::vector<T, PoolAllocator<T> >::push_back(a);
}

// Extend with push_front for std::vector
template <class T>
inline void Vector<T>::push_back(const Vector<T>& a)
{
    for (int i=0; i<a.size(); i++)
    {
        std::vector<T, PoolAllocator<T> >::push_back(a[i]);
    }
}

// Extend Vector class with find method
template <class T>
inline int Vector<T>::find(const T& a) const
{
    for (int i=0; i<size(); i++)
    {
        if (this->at(i)==a)  return i;
    }
    return -1;
}

template <class T>
inline void Vector<T>::reallocate(int size)
{
    reallocate(size, T());
}

template <class T>
inline void Vector<T>::reallocate(int size, const T& a)
{
    int old_size = this->size();
    int new_size = size;
    Vector<T> buff = *this;     // save current std::vector
    this->assign(new_size, a);        // resize current std::vector
    for (int i=0; i<std::min(old_size,new_size); i++)
    {
        this->at(i) = buff[i];
    }
    buff.clear();
}

// Extend Vector class with << method
template <class T>
std::ostream& operator << (std::ostream& os, const Vector<T>& v)
{
    for (int i=0; i<v.size(); i++)   os << v[i] << "\n";
    return os;
}

//typedef const Vector<bool> Vecbool_I;
//typedef Vector<bool> Vecbool, Vecbool_O, Vecbool_IO;


#include "simplemathvector.hpp"
/*
typedef const Vector<int> Vecbool_I;
typedef Vector<int> Vecbool, Vecbool_O, Vecbool_IO;
*/


/*
    Under most STL implementations stl::std::vector<bool> is specialized to implement bitfield compression, while this divides the 
    size of the storage by 8 it also generally reduces performance(by adding a slight overhead to acces individual bits in the bitfield)
    To avoid this when possible the Vecbool is actually implemented as a std::vector of ints. While this does not change the code logic it 
    can raise warnings with microsoft compiler, hence this optimization is not active for windows compilation.
*/


typedef const SimpleMathsVector<bool> VecBool_I;
typedef SimpleMathsVector<bool> VecBool, VecBool_O, VecBool_IO;

/*#ifndef WIN32
typedef const SimpleMathsVector<bool> Vecbool_I;
typedef SimpleMathsVector<bool> Vecbool, Vecbool_O, Vecbool_IO;
#else
typedef const SimpleMathsVector<bool> Vecbool_I;
typedef SimpleMathsVector<bool> Vecbool, Vecbool_O, Vecbool_IO;
#endif*/


#endif
