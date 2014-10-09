#ifndef __RTE_SLUB_H__
#define __RTE_SLUB_H__
#include "rte_list.h"
#include "rte_spinlock.h"

struct mem_cache_cpu{
	void **freelist; // 指向本地Local slab的空闲Obj链表
	struct rte_page *page;
};

struct mem_cache_node{
	rte_spinlock_t list_lock;
	unsigned long nr_partial;
	struct list_head partial;
};

#define RTE_SLAB_BASE_SIZE 64
#define RTE_SHM_CACHE_NUM 14
#define RTE_LOCAL_CACHE_NUM 14 //Max 512K
#define RTE_OO_SHIFT 16
#define RTE_OO_MASK ((1UL<<RTE_OO_SHIFT)-1)

struct rte_mem_cache{
	struct mem_cache_cpu cpu_slab[RTE_MAX_CPU_NUM];
	int size; 
	int offset; 
	int objsize; 
	unsigned long oo;
	struct mem_cache_node local_node;
	unsigned long min_partial;
	unsigned long alloc_cnt;
	unsigned long free_cnt;
};

static inline void RTE_SLUB_BUG(const char *name, int line)
{
	printf("SLUB_BUG On file %s line %d.\n", name, line);	
	assert(0);
}

int rte_slub_system_init(struct rte_mem_cache *array, int cache_num, int node_id);
void * __rte_slub_alloc(uint32_t size, int node_id);
void __rte_slub_free(void *ptr, int node_id);


#endif