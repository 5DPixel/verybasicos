#![no_std]

extern crate alloc;

use core::alloc::{GlobalAlloc, Layout};

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}

unsafe extern "C" {
	fn kernel_heap_alloc_pages(pages: isize);
	fn kernel_heap_free_pages(pages: isize, pages: *mut u8);
	
	fn kernel_heap_alloc(size: usize) -> *mut u8;
	fn kernel_heap_free(ptr: *mut u8, size: usize);
}

struct KernelAllocator;

unsafe impl GlobalAlloc for KernelAllocator {
	unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
		kernel_heap_alloc(layout.size())
	}
	
	unsafe fn dealloc(&self, ptr: *mut u8, layout: Layout){
		kernel_heap_free(ptr, layout.size());
	}
}

#[global_allocator]
static ALLOCATOR: KernelAllocator = KernelAllocator;
