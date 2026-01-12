#ifndef BOOT_SHARED_PLATFORM_H
#define BOOT_SHARED_PLATFORM_H

#include <stddef.h>
#include <stdint.h>
#include <common.h>
#include <boot/shared/log.h>
#include <kernel.h>

/* common colours */
#define PLATFORM_WHITE 0xFFFFFFFF
#define PLATFORM_BLACK 0x00000000

#define TEXT_ATTR(fg, bg) fg | (bg >> 8)
#define BOOT_MENU_TEXT_ATTR TEXT_ATTR(PLATFORM_WHITE, PLATFORM_BLUE)

#define MAX_BUFFER_LOGS 128 /* initial hardcoded for testing */

struct text_attributes;
struct framebuffer;
enum log_severity;

struct platform_model {
	void *(*alloc)(struct platform_model *model, size_t size);
	void *(*alloc_conventional)(struct platform_model *model, size_t size);
	void (*alloc_pages)(struct platform_model *model, int page_count, uint64_t addr);
	void (*exit)(struct platform_model *model);
	void (*free)(struct platform_model *model, void *ptr);
	void (*write)(struct platform_model *model, const char *message, struct text_attributes *attr);
	void (*log)(struct platform_model *model, char *message, enum log_severity severity);
	void (*font_dimensions)(struct platform_model *model, int *width, int *height);
	struct mmap_entry *(*mmap_entries)(struct platform_model *model, int *mmap_entry_count);
	uint8_t *(*read_file)(struct platform_model *model, const char *file_name, uint32_t *size);
	void (*plot32)(struct platform_model *model, int x, int y, uint32_t colour);
	struct framebuffer *(*display_attributes)(struct platform_model *model);
	enum key (*get_key)(struct platform_model *model); /* polling */
	enum platform_type (*platform)(struct platform_model *model);
	void (*clear_screen)(struct platform_model *model, uint32_t colour);
	void *ctx;
	struct log_circular_buffer *internal_log_buffer;
	struct text_attributes *text_attr;
	uint8_t *font;
	uint8_t *kernel;
	uint32_t font_length;
	uint32_t kernel_length;
};

enum key {
	KEY_SPACE = 0x00,
    KEY_UP_ARROW = 0x01,
    KEY_DOWN_ARROW = 0x02
};

enum platform_type {
	PLATFORM_BIOS = 0,
	PLATFORM_UEFI = 1
};

/* platform-specific functions */
void *platform_alloc(struct platform_model *model, size_t size);
void platform_alloc_pages(struct platform_model *model, int page_count, uint64_t addr);
void platform_free(struct platform_model *model, void *ptr);
void platform_write(struct platform_model *model, const char *message, struct text_attributes *attr);
void platform_exit(struct platform_model *model);
struct framebuffer *platform_display_attributes(struct platform_model *model);
struct mmap_entry *platform_get_mmap_entries(struct platform_model *model, int *mmap_entry_count);
uint8_t *platform_read_file(struct platform_model *model, const char *file_name, uint32_t *size);
void init_platform(void *ctx, const char *font_file_path, const char *kernel_file_path, struct platform_model *model);
enum platform_type platform(struct platform_model *model);
enum key get_key(struct platform_model *model); /* polling, this function should later be prefixed with platform_ */

void set_text_attr(struct platform_model *model, uint32_t text_attr);
void plot_pixel_32bpp(struct platform_model *model, int x, int y, uint32_t colour);
void open_font_file(struct platform_model *model, const char *file);
void log(struct platform_model *model, char *message, enum log_severity severity);
void font_dimensions(struct platform_model *model, int *width, int *height);
void clear_screen(struct platform_model *model, uint32_t colour);

#endif
