.SUFFIXES:

BUILD_DIR := $(CURDIR)/../../../../build/boot
OBJ_DIR := $(CURDIR)/../../../../obj/boot
INCLUDE_DIR := $(CURDIR)/../../../../include
GNU_EFI_SUBMODULE = $(CURDIR)/../../../../gnu-efi

override OUTPUT_BINARY := $(BUILD_DIR)/boot.efi

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

LOG_QUIET := 1

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
	-I$(GNU_EFI_SUBMODULE)/inc \
	-I$(GNU_EFI_SUBMODULE)/inc/x86_64 \
	-I$(GNU_EFI_SUBMODULE)/inc/protocol \
	-I$(INCLUDE_DIR)/x86_64/boot \
	-I$(INCLUDE_DIR)/shared \
    $(CPPFLAGS) \
	-DEFI_FUNCTION_WRAPPER \
    -MMD \
    -MP

override LDFLAGS += \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    --gc-sections \
    -T $(LINKER_SCRIPT)

override SRC := $(shell find $(CURDIR)/../../../shared/boot/uefi $(CURDIR)/../../../shared/boot/shared -type f 2>/dev/null | LC_ALL=C sort)
override C_SRC := $(filter %.c, $(SRC))
override OBJ := $(addprefix $(OBJ_DIR)/, $(C_SRC:.c=.c.o)) # remember to use the slash, otherwise the path gets formatted as ../../obj/boot./[file]
override HEADER_DEPS := $(addprefix $(OBJ_DIR), $(C_SRC:.c=.c.d))

.PHONY: all
all: $(OUTPUT_BINARY)

-include $(HEADER_DEPS)

$(OUTPUT_BINARY): GNUmakefile $(OBJ)
	@mkdir -p "$(dir $@)"
	$(call log_step, \
		$(LD) $(LDFLAGS) $(OBJ) -o $@, \
		$(OBJ), \
		$@ \
	)

$(OBJ_DIR)/%.c.o: %.c GNUmakefile
	@mkdir -p "$(dir $@)"
	$(call log_step, \
		$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@, \
		$<, \
		$@ \
	)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
