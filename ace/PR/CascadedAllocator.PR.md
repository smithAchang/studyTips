`ACE` has fixed-size `ACE*_Cached_Allocator`, but in some case the total `n_chunks` can't be determined easyly :(

If the `ACE` framework has the ability to malloc fixed-size chunks 'infinitely' just like `ACE_Malloc< ACE_MEM_POOL_1, ACE_LOCK >` for mallocing various-size chunks, that will be very useful!

# design

+ choose `ACE_Dynamic_Cached_Allocator`  not `ACE_Cached_Allocator` 
> `ACE_Cached_Allocator` provides poor enhancement for the API 
+ combinate `ACE_Vector<T>` to enable the flexibility of allocator hierarchy 
+ all allocators forms a hierarchy with the allocator in lower level has `2 * n_chunks(upper level)`  capacity

