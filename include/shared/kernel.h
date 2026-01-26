#ifndef SHARED_KERNEL_H
#define SHARED_KERNEL_H

#define MAX_MMAP_ENTRY_COUNT 256

enum mem_type {
	MEM_FREE = 0,
	MEM_RESERVED,
	MEM_ACPI,
	MEM_MMIO,
	MEM_KERNEL,
	MEM_BOOT,
	MEM_BAD
};

struct acpi_table {
	uintptr_t acpi_start;
	uint32_t acpi_revision;
};

struct mmap_entry {
	uint64_t phys_base;
	uint64_t virt_base;
	uint64_t size;
	enum mem_type type;
} __attribute__((packed));

struct kernel_boot_params {
	uint8_t *font;
	struct framebuffer *fb;
	uint32_t mem_region_count;
	struct mmap_entry *mem_regions;
	struct acpi_table *acpi;
} __attribute__((packed));

void kernel_init(struct kernel_boot_params *params);

#endif
