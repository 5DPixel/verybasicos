.SUFFIXES:

BUILD_DIR := $(CURDIR)/../../../../build/boot
OBJ_DIR := $(CURDIR)/../../../../obj/boot
INCLUDE_DIR := $(CURDIR)/../../../../include
GNU_EFI_SUBMODULE = $(CURDIR)/../../../../gnu-efi

override OUTPUT_BINARY := $(BUILD_DIR)/boot.efi

TOOLCHAIN :=
TOOLCHAIN_PREFIX := x86_64-w64-mingw32-

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

override CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep -q '^Target: '; echo $$?)

ifeq ($(CC_IS_CLANG),1)
    override CC += \
        -target x86_64-windows
endif

override CFLAGS += \
    -Wall \
    -Wextra \
    -std=c11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fPIC \
	-fshort-wchar \
    -m64 \
    -march=x86-64 \
    -mno-red-zone \
	-maccumulate-outgoing-args

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
	-dll \
	-e efi_main \
    -shared \
	--subsystem=10 \
	-L$(GNU_EFI_SUBMODULE)/x86_64/lib \
	-L$(GNU_EFI_SUBMODULE)/x86_64/gnuefi \
	-lgnuefi \
	-lefi

#override SRC := $(shell find -type f 2>/dev/null | LC_ALL=C sort)
override SRC := $(shell find $(CURDIR)/../../../shared/boot/uefi $(CURDIR)/../../../shared/boot/shared -type f 2>/dev/null | LC_ALL=C sort)
override C_SRC := $(filter %.c, $(SRC))
override OBJ := $(addprefix $(OBJ_DIR)/, $(C_SRC:.c=.c.o)) # remember to use the slash, otherwise the path gets formatted as ../../obj/boot./[file]
override HEADER_DEPS := $(addprefix $(OBJ_DIR), $(C_SRC:.c=.c.d))

.PHONY: all
all: $(OUTPUT_BINARY)

-include $(HEADER_DEPS)

$(OUTPUT_BINARY): GNUmakefile $(OBJ)
	mkdir -p "$(dir $@)"
	$(LD) $(LDFLAGS) $(OBJ) -o $@

$(OBJ_DIR)/%.c.o: %.c GNUmakefile
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
