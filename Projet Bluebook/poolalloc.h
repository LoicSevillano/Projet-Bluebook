#ifndef POOLALLOC_H  
#define POOLALLOC_H

#include <malloc.h>
#include <stdio.h>
#include "poolstorage.h"

template<typename _Tp>
    class PoolAllocator;

  /// PoolAllocator<void> specialization.
  template<>
          class PoolAllocator<void>
{
    public:
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef void*       pointer;
        typedef const void* const_pointer;
        typedef void        value_type;

        template<typename _Tp1>
                struct rebind
        { typedef PoolAllocator<_Tp1> other; };
};


  template<typename _Tp>
          class PoolAllocator
{
    public:
        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;
        typedef _Tp*       pointer;
        typedef const _Tp* const_pointer;
        typedef _Tp&       reference;
        typedef const _Tp& const_reference;
        typedef _Tp        value_type;

        template<typename _Tp1>
                struct rebind
        { typedef PoolAllocator<_Tp1> other; };

        PoolAllocator() throw() { }

        PoolAllocator(const PoolAllocator& __a) throw() { }

        template<typename _Tp1>
                PoolAllocator(const PoolAllocator<_Tp1>&) throw() { }

        ~PoolAllocator() throw() { }
        
        
        pointer address(reference x) const {return &x;}
        const_pointer address(const_reference x) const {return &x;}
        
        // NB: __n is permitted to be 0.  The C++ standard says nothing
      // about what the return value is when __n == 0.
        pointer
                allocate(size_type __n, const void* = 0)
        { 
            
            void* ptr=pool_alloc(__n * sizeof(_Tp));
            
            return static_cast<_Tp*>(ptr);
        }

      // __p is not permitted to be a null pointer.
        void
                deallocate(pointer __p, size_type)
        { 
            
            pool_free(__p);
            
        }

        size_type
                max_size() const throw() 
        { return size_t(-1) / sizeof(_Tp); }

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 402. wrong new expression in [some_] allocator::construct
        void 
                construct(pointer __p, const _Tp& __val) 
        { 
            //printf("constructing pointer %p\n",__p);
                ::new(__p) _Tp(__val); 
        }

        void 
                destroy(pointer __p) 
        {
            //printf("destroying pointer %p\n",__p); 
            __p->~_Tp(); 
        }
};

    template<typename _T1, typename _T2>
            inline bool
                    operator==(const PoolAllocator<_T1>&, const PoolAllocator<_T2>&)
    { return true; }

    template<typename _T1, typename _T2>
            inline bool
                    operator!=(const PoolAllocator<_T1>&, const PoolAllocator<_T2>&)
            { return false; }
                  
                  
#endif                  


