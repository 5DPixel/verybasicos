#ifndef KERNEL_X86_64_PAGING_H
#define KERNEL_X86_64_PAGING_H

static inline void native_invalidate_page(void *page){
	__asm__ volatile("invlpg (%0)" : : "b"(page) : "memory" );
}

#endif
