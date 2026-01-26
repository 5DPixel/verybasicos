.SUFFIXES:

BUILD_DIR := ../../../../build/boot
OBJ_DIR := ../../../../obj/boot
INCLUDE_DIR := ../../../../include

override OUTPUT_MBR := $(BUILD_DIR)/boot_sect
override OUTPUT_STAGE1 := $(BUILD_DIR)/boot
override OUTPUT_STAGE2 := $(BUILD_DIR)/boot_stage2

override STAGE1_LINKER_SCRIPT := boot.ld
override STAGE2_LINKER_SCRIPT := stage2/boot.ld

TOOLCHAIN :=
TOOLCHAIN_PREFIX :=

ifneq ($(TOOLCHAIN),)
    ifeq ($(TOOLCHAIN_PREFIX),)
        TOOLCHAIN_PREFIX := $(TOOLCHAIN)-
    endif
endif

ifneq ($(TOOLCHAIN_PREFIX),)
    CC := $(TOOLCHAIN_PREFIX)gcc
else
    CC := cc
endif

LD := $(TOOLCHAIN_PREFIX)ld

ifeq ($(TOOLCHAIN),llvm)
    CC := clang
    LD := ld.lld
endif

CFLAGS := -g -O2 -pipe
CPPFLAGS :=
LDFLAGS :=
FASMFLAGS :=

LOG_QUIET := 0

# arg 1: command, arg 2: source, arg 3: out
define log_step
    $(if $(filter $(LOG_QUIET), 1),
		@$(1) 2>/dev/null 1>/dev/null;
		@echo "$(firstword $(subst :, , $(1)))       $(shell basename -a $(2))       $(shell basename -a $(3))",
		$(1)
    )
endef

override CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep -q '^Target: '; echo $$?)

ifeq ($(CC_IS_CLANG),1)
    override CC += \
        -target x86_64-unknown
endif

override CFLAGS += \
    -Wall \
	-Wextra \
    -std=c11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIC \
    -fno-strict-aliasing \
    -ffunction-sections \
    -fdata-sections \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone

override CPPFLAGS := \
	-I$(INCLUDE_DIR)/x86_64/boot \
	-I$(INCLUDE_DIR)/shared \
    $(CPPFLAGS) \
    -MMD \
    -MP

override LDFLAGS += \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    --gc-sections

BOOT_ENTRY_ASM_FILE := boot_sect/start.asm
SHARED_DIRECTORY := ../../../shared/boot/shared

override STAGE1_SRC := $(shell find . -type f 2>/dev/null -not -path "./boot_sect/*" | LC_ALL=C sort)
override STAGE1_C_SRC := $(filter %.c, $(STAGE1_SRC))
override STAGE1_ASM_SRC := $(filter %.asm, $(STAGE1_SRC))
override STAGE1_OBJ := $(addprefix $(OBJ_DIR)/, $(STAGE1_C_SRC:.c=.c.o) $(STAGE1_ASM_SRC:.asm=.asm.o)) # remember to use the slash, otherwise the path gets formatted as ../../obj/boot./[file]
override HEADER_DEPS := $(addprefix $(OBJ_DIR), $(C_SRC:.c=.c.d))

#override STAGE2_SRC := $(shell find ../../../shared/boot/bios ../../../shared/boot/shared . -type f 2>/dev/null -not -path $(BOOT_ENTRY_ASM_FILE) | LC_ALL=C sort) TODO: uncomment later

override STAGE2_SRC := $(shell find stage2 -type f 2>/dev/null | LC_ALL=C sort)
override STAGE2_C_SRC := $(filter %.c, $(STAGE2_SRC))
override STAGE2_ASM_SRC := $(filter %.asm, $(STAGE2_SRC))
override STAGE2_OBJ := $(addprefix $(OBJ_DIR)/, $(STAGE2_C_SRC:.c=.c.o) $(STAGE2_ASM_SRC:.asm=.asm.o))

.PHONY: all
all: $(OUTPUT_STAGE2)
-include $(HEADER_DEPS)

# define stage 2 target, depends on stage 1

$(OUTPUT_STAGE2): GNUmakefile $(OUTPUT_STAGE1) $(STAGE2_OBJ)
	@mkdir -p "$(dir $@)"
	$(call log_step, \
		$(LD) $(LDFLAGS) -T$(STAGE2_LINKER_SCRIPT) $(STAGE2_OBJ) -o $@, \
		$(STAGE2_OBJ), \
		$@ \
	)

$(OUTPUT_STAGE1): GNUmakefile $(STAGE1_OBJ) $(BOOT_ENTRY_ASM_FILE) $(LINKER_SCRIPT)
	@mkdir -p "$(dir $@)"
	echo $(STAGE1_OBJ)
	$(call log_step, \
		fasm $(BOOT_ENTRY_ASM_FILE) $(OUTPUT_MBR), \
		$(BOOT_ENTRY_ASM_FILE), \
		$(OUTPUT_MBR) \
	)
	
	$(call log_step, \
		$(LD) $(LDFLAGS) -T$(STAGE1_LINKER_SCRIPT) $(STAGE1_OBJ) -o $@, \
		$(STAGE1_OBJ), \
		$@ \
	)

$(OBJ_DIR)/%.c.o: %.c GNUmakefile
	@mkdir -p "$(dir $@)"
	$(call log_step, \
		$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@, \
		$<, \
		$@ \
	)

$(OBJ_DIR)/%.asm.o: %.asm GNUmakefile
	@mkdir -p "$(dir $@)"
	$(call log_step, \
		fasm $(FASMFLAGS) $< $@, \
		$<, \
		$@ \
	)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
