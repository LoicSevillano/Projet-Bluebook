#include "poolstorage.h"
#include <malloc.h>
//#include "macro.hpp"
#include <stdio.h>  
#include <stdlib.h>
#include <string>
#include <stdint.h>

//this code is not thread safe, deactivate this code because 
//will be problematic with threaded version:
#define USE_MALLOC_DIRECTLY

#ifndef USE_MALLOC_DIRECTLY
//define 6 pools:8,16,32,64,128,256
#define POOL_COUNT  6 
#define MEGABYTE (1024*1024)
//use fast bit scan(for x86 ort x86_64)

#ifndef WIN32
#define USE_X86_BSR 
#endif


#ifndef USE_X86_BSR
inline unsigned int int_log2( unsigned int x )//Slow bitscan reverse (portable way)
{
    unsigned int ans = 0 ;
    while( x>>=1 ) ans++;
    return ans ;
}//*/
#else
//fast log2 operation using directly bitscanreverse instruction (for X86)
inline int int_log2(int x) 
{ 
    int msb;
    asm("bsrl %1,%0" : "=r"(msb) : "r"(x)); //some assembly to use BSR instruction
    return msb;
}
#endif
#endif    

#ifndef USE_MALLOC_DIRECTLY
typedef struct t_free_list_node
{
    union
    {
        char data[1];
        struct t_free_list_node* next;
    };
    
}t_free_list_node;

static const size_t s_def_pool_sizes[6]={   32*MEGABYTE,   //for 8 byte objects
                                            4*MEGABYTE,    //for 16 byte objects
                                            32*MEGABYTE,   //for 32 byte objects 
                                            4*MEGABYTE,    //for 64 byte objects
                                            16*MEGABYTE,    //for 128 byte objects
                                            16*MEGABYTE};   //for 256 byte objects

static t_free_list_node*   s_free_list_head[POOL_COUNT];
        
static void*               s_buf_ptr[POOL_COUNT];

static uint64_t            s_buf_ofs[POOL_COUNT+1];
        
        
static size_t              s_elmnt_size[POOL_COUNT];
static size_t              s_elmnt_count[POOL_COUNT];
        
        
        
static void *              s_glbl_buffer=NULL;
static size_t              s_glbl_buf_size=0;

static uint64_t            s_pool_alloc_cnt[POOL_COUNT];
static uint64_t            s_pool_free_cnt[POOL_COUNT];
static uint64_t            s_pool_alloc_fail_cnt[POOL_COUNT];

static uint64_t            s_out_of_pool_alloc_cnt=0;
static uint64_t            s_out_of_pool_free_cnt=0;


static uint64_t            s_alloc_cnt=0;
static uint64_t            s_free_cnt=0;

static int s_pool_initialized=0;
#endif


#ifndef USE_MALLOC_DIRECTLY
void PoolAllocSystem_build_initial_linked_list(t_free_list_node** list_head,size_t size,size_t count,char* buffer)
{
    size_t i;
    
    
    //build initial linked list
    for(i=0;i<count-1;i++)
    { 
        
        t_free_list_node* cur_node=(t_free_list_node*) &buffer[i*size];
        t_free_list_node* next_node=(t_free_list_node*) &buffer[(i+1)*size];
        
        cur_node->next=next_node;
        
    }
    
    //set end of list
    t_free_list_node* last_node=(t_free_list_node*) &buffer[(count-1)*size];
    last_node->next=NULL;
    
    //set list head
    (*list_head)=(t_free_list_node*) &buffer[0];
    
}


void PoolAllocSystem_createPools(const size_t* pool_mem_size)
{
    //check params
    BEST_DEBUG_ASSERT(pool_mem_size!=NULL);
    
    
    
    
    int i;
    
    s_glbl_buf_size=0;
    
    for(i=0;i<POOL_COUNT;i++)
    {
        s_glbl_buf_size+=pool_mem_size[i];
    }
    
    
    s_glbl_buffer=malloc(s_glbl_buf_size);
    
    if(s_glbl_buffer==NULL)
    {
        fprintf(stderr,"Unable to preallocate memory for pool allocator, memory required=%libytes\n",s_glbl_buf_size);
        abort();
    }
    
    
    s_buf_ptr[0]=s_glbl_buffer;
    
    s_buf_ofs[0]=(uint64_t)s_glbl_buffer;
    
    for(i=0;i<POOL_COUNT-1;i++)
    {
        s_buf_ptr[i+1]=(void*)(((uint64_t)s_buf_ptr[i])+(uint64_t)pool_mem_size[i]);
        
        s_buf_ofs[i+1]=s_buf_ofs[i]+pool_mem_size[i];
    }
    
    s_buf_ofs[POOL_COUNT]=s_buf_ofs[POOL_COUNT-1]+pool_mem_size[POOL_COUNT-1];
    
    //create each individual pool using pool_mes_size to determine the memory to reserve
    size_t element_size=8;
    
    for(i=0;i<POOL_COUNT;i++)
    {
        
        
        s_elmnt_size[i]=element_size;
        s_elmnt_count[i]=pool_mem_size[i]/element_size;
        
        PoolAllocSystem_build_initial_linked_list(&s_free_list_head[i],s_elmnt_size[i],
                                   s_elmnt_count[i],(char*)s_buf_ptr[i]);
        
        //update element_size
        element_size*=2;
    }
    
    
    
}


inline void* PoolAllocSystem_allocate_pool(int pool)
{
        
    t_free_list_node* next=s_free_list_head[pool]->next;
            
    void* ptr=&s_free_list_head[pool]->data[0];
            
    s_free_list_head[pool]=next;
    
    s_pool_alloc_cnt[pool]++;
            
    return ptr;
}
    
inline void PoolAllocSystem_deallocate_pool(int pool,void* ptr)
{

    t_free_list_node* p=(t_free_list_node*)ptr;
            
    p->next=s_free_list_head[pool];
            
    s_free_list_head[pool]=p;
    
    s_pool_free_cnt[pool]++;
}
#endif


void PoolAllocSystem_Init()
{
#ifndef USE_MALLOC_DIRECTLY       
    memset(s_pool_alloc_cnt,0,sizeof(s_pool_alloc_cnt));
    memset(s_pool_free_cnt,0,sizeof(s_pool_free_cnt));
    memset(s_pool_alloc_fail_cnt,0,sizeof(s_pool_alloc_fail_cnt));
 
    PoolAllocSystem_createPools(s_def_pool_sizes);

    s_pool_initialized=1;
#endif        
}


void * PoolAllocSystem_allocate(size_t buf_size)
{
#ifndef USE_MALLOC_DIRECTLY
    if(s_pool_initialized==0)
    {
        PoolAllocSystem_Init();
    }
    
    s_alloc_cnt++;
    
    if(buf_size<=8)
    {
        if(s_free_list_head[0]!=NULL)
        {
            return PoolAllocSystem_allocate_pool(0);
        }
        else
        {
            s_pool_alloc_fail_cnt[0]++;
            s_out_of_pool_alloc_cnt++;
            
            return malloc(buf_size);
        }
        
    }
    else
    {

        int log2_size=int_log2((unsigned int)(buf_size-1));

        int pool=log2_size-2; //first pool is for size 8 (log2(8)=3),add one to use large enough pool


        if(pool<POOL_COUNT)
        {
            if(s_free_list_head[pool]!=NULL)
            {
                return PoolAllocSystem_allocate_pool(pool);
            }
            else
            {
                
                s_pool_alloc_fail_cnt[pool]++;
                s_out_of_pool_alloc_cnt++;
                
                return malloc(buf_size);
            }
        }
    }
 
    s_out_of_pool_alloc_cnt++;
#endif    
    return malloc(buf_size);
}


void PoolAllocSystem_deallocate( void* ptr)
{
#ifndef USE_MALLOC_DIRECTLY
    s_free_cnt++;
    
    if( ((uint64_t)ptr)>=s_buf_ofs[0] && ((uint64_t)ptr)<s_buf_ofs[1] ) 
    {
        PoolAllocSystem_deallocate_pool(0,ptr);
        return;
    }
    
    if( ((uint64_t)ptr)>=s_buf_ofs[1] && ((uint64_t)ptr)<s_buf_ofs[2] ) 
    {
        PoolAllocSystem_deallocate_pool(1,ptr);
        return;
    }
    
    
    if( ((uint64_t)ptr)>=s_buf_ofs[2] && ((uint64_t)ptr)<s_buf_ofs[3] ) 
    {
        PoolAllocSystem_deallocate_pool(2,ptr);
        return;
    }
    
    if( ((uint64_t)ptr)>=s_buf_ofs[3] && ((uint64_t)ptr)<s_buf_ofs[4] ) 
    {
        PoolAllocSystem_deallocate_pool(3,ptr);
        return;
    }
    
    if( ((uint64_t)ptr)>=s_buf_ofs[4] && ((uint64_t)ptr)<s_buf_ofs[5] ) 
    {
        PoolAllocSystem_deallocate_pool(4,ptr);
        return;
    }
    
    if( ((uint64_t)ptr)>=s_buf_ofs[5] && ((uint64_t)ptr)<s_buf_ofs[6] ) 
    {
        PoolAllocSystem_deallocate_pool(5,ptr);
        return;
    }
    
#endif
    free(ptr);
#ifndef USE_MALLOC_DIRECTLY      
    s_out_of_pool_free_cnt++;
#endif       
}


void PoolAllocSystes_destroyPools()
{
#ifndef USE_MALLOC_DIRECTLY  
    if(s_glbl_buffer!=NULL) free(s_glbl_buffer);
    s_glbl_buffer=NULL;
#endif
}

void PoolAllocSystem_dump_stats()
{
#ifndef USE_MALLOC_DIRECTLY
    fprintf(stderr,"PoolAlloc stats:\n");
    fprintf(stderr," - Total allocs %li\n",s_alloc_cnt);
    fprintf(stderr," - Total frees %li\n",s_free_cnt);
    
    for (int i=0;i<POOL_COUNT;i++)
    {
        
        fprintf(stderr,"   - pool %i - size %li - allocs %li - allocs failed %li - frees %li \n",
                i,s_elmnt_size[i],s_pool_alloc_cnt[i],s_pool_alloc_fail_cnt[i],s_pool_free_cnt[i]);
        
    }
    
    fprintf(stderr," - Out of Pool allocs %li\n",s_out_of_pool_alloc_cnt);
    fprintf(stderr," - Out of Pool frees %li\n",s_out_of_pool_free_cnt);
#endif    
}
