#include <lib/Clock.hpp>
#include <lib/RWLock.hpp>   
#include <lib/ac_pool.hpp>
#include <iostream>
#include <memory>
#include <string.h>
using namespace std;
#define __rte_cache_aligned __attribute__((__aligned__(64)))
struct rte_ring {
	char pad0; /**< empty cache line */
    u_int32 i;
    u_int32 j;
};
abstract_container::pool<rte_ring, unsigned int, 2> pool;
int main(int argc, char** argv)
{  
    std::cout<<"size:"<<sizeof(unsigned int)<<std::endl;
    std::cout<<"size:"<<sizeof(rte_ring)<<std::endl;
    std::cout<<"size:"<<sizeof(pool)<<std::endl;
    auto i = pool.alloc();
    std::cout<<"alloc:"<<i<<std::endl;
    std::cout<<"can_alloc:"<<pool.can_alloc()<<std::endl;
    auto j = pool.alloc();
    std::cout<<"alloc:"<<j<<std::endl;
    std::cout<<"can_alloc:"<<pool.can_alloc()<<std::endl;
    return 0;  

}  