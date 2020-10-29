#ifndef poolstorage_h
#define poolstorage_h

#include <stddef.h>

    
void PoolAllocSystem_createPools(const size_t* pool_mem_size);
      
void PoolAllocSystem_Init();

void PoolAllocSystem_dump_stats();

void * PoolAllocSystem_allocate(size_t buf_size);
void PoolAllocSystem_deallocate( void* ptr);


static inline void * pool_alloc(size_t s)
{
    return PoolAllocSystem_allocate(s);
}


static inline void pool_free(void * ptr)
{
    PoolAllocSystem_deallocate(ptr);
}

#endif