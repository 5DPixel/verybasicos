.SUFFIXES:

override OUTPUT_BOOTLOADER = $(CURDIR)/build/boot/boot.efi
override OUTPUT_ISO = $(CURDIR)/os.iso
override OUTPUT_ESP = $(CURDIR)/build/boot/esp.img
override OUTPUT_KERNEL = $(CURDIR)/build/kernel

BOOT_TOOLCHAIN :=
BOOT_TOOLCHAIN_PREFIX := x86_64-w64-mingw32-

SKIP_TOOL_CHECK := 0

CPUS := 1

KERNEL_TOOLCHAIN :=
KERNEL_TOOLCHAIN_PREFIX :=
FONT_NAME := spleen-8x16.bdf
FONT := $(addprefix $(CURDIR)/spleen/, $(FONT_NAME))
FONT_OUT := $(addprefix $(CURDIR)/assets/fonts/, $(FONT_NAME:.bdf=.psf))

ifeq ($(ARCH_EXISTS), 0)
	$(error "Arch $(ARCH) doesn't exist, specify another!")
endif

LOG_QUIET := 1

# arg 1: command, arg 2: source, arg 3: out
define log_step
    $(if $(filter $(LOG_QUIET), 1),
        @$(1) 2>/dev/null 1>/dev/null;
        @echo "$(firstword $(subst :, , $(1)))        $(shell basename -a $(2))        $(shell basename -a $(3))",
        $(1)
    )
endef

define log_step_nofile
    $(if $(filter $(LOG_QUIET), 1),
        @$(1);
        @echo "$(firstword $(subst :, , $(1)))    $(2)    $(3)",
        $(1)
    )
endef

define log_if_notquiet
	$(if $(filter $(LOG_QUIET), 1),
		@$(1) 2>/dev/null 1>/dev/null,
		$(1)
	)
endef

define add_flag_if_quiet
	$(if $(filter $(LOG_QUIET), 1),
		@$(1) $(2),
		$(1)
	)
endef

-include config

.PHONY: all
all: $(OUTPUT_ISO)

$(OUTPUT_ISO): GNUmakefile submodules
	@$(if $(filter $(SKIP_TOOL_CHECK),0), $(CURDIR)/scripts/check_tools.py --exit,)
	@$(CURDIR)/scripts/check_tools.py --check-arch $(CURDIR)/arch $(ARCH)
	$(call add_flag_if_quiet, \
		$(MAKE) -C $(CURDIR)/arch/$(ARCH)/boot/uefi \
			-j$(CPUS) TOOLCHAIN_PREFIX=$(BOOT_TOOLCHAIN_PREFIX) \
			TOOLCHAIN=$(BOOT_TOOLCHAIN) LOG_QUIET=$(LOG_QUIET), \
		 --no-print-directory -s \
	)
	$(call add_flag_if_quiet, $(MAKE) -C $(CURDIR)/arch/$(ARCH) -j$(CPUS) LOG_QUIET=$(LOG_QUIET), --no-print-directory -s)
	$(call log_if_notquiet, dd if=/dev/zero of=$(OUTPUT_ESP) bs=1k count=100000)
	$(call log_if_notquiet, mkdosfs -F 32 $(OUTPUT_ESP))
	$(call log_if_notquiet, mmd -i $(OUTPUT_ESP) ::/EFI)
	$(call log_if_notquiet, mmd -i $(OUTPUT_ESP) ::/EFI/BOOT)
	$(call log_step, \
		mcopy -i $(OUTPUT_ESP) $(OUTPUT_BOOTLOADER) ::/EFI/BOOT/BOOTX64.EFI, \
		$(OUTPUT_ESP), \
		$(OUTPUT_BOOTLOADER), \
	)
	$(call log_step, \
		mcopy -i $(OUTPUT_ESP) $(OUTPUT_KERNEL) ::/EFI/BOOT/kernel, \
		$(OUTPUT_ESP), \
		$(OUTPUT_KERNEL) \
	)
	$(call log_step, \
		mcopy -i $(OUTPUT_ESP) $(FONT_OUT) ::/EFI/BOOT/spleen.psf, \
		$(OUTPUT_ESP), \
		$(FONT_OUT) \
	)
	@mkdir -p iso
	$(call log_if_notquiet, cp $(OUTPUT_ESP) iso)
	$(call log_step, \
		xorriso \
			-as mkisofs \
			-R \
			-f \
			-e esp.img \
			-no-emul-boot \
			-o $(OUTPUT_ISO) \
			iso, \
		$(OUTPUT_ISO), \
		iso \
	)
	@echo "build complete..."

.PHONY: clean
clean:
	rm -f $(OUTPUT_ESP)
	rm -f $(OUTPUT_BOOTLOADER)
	rm -f $(OUTPUT_ISO)
	rm -rf $(CURDIR)/build
	rm -rf $(CURDIR)/obj
	rm -rf $(CURDIR)/assets/fonts

.PHONY: distclean
distclean:
	rm -f $(OUTPUT_ESP)
	rm -f $(OUTPUT_BOOTLOADER)
	rm -f $(OUTPUT_ISO)
	rm -rf $(CURDIR)/build
	rm -rf $(CURDIR)/obj
	rm -rf $(CURDIR)/assets/fonts
	git submodule foreach --recursive make clean || true

.PHONY: submodules
submodules:
	$(call add_flag_if_quiet, $(MAKE) -C gnu-efi -j$(CPUS), --no-print-directory -s)
	@mkdir -p assets/fonts
	$(call log_step, \
		bdf2psf --fb $(FONT) \
			/usr/share/bdf2psf/standard.equivalents \
			/usr/share/bdf2psf/ascii.set \
			256 \
			$(FONT_OUT), \
		/usr/share/bdf2psf/standard.equivalents, \
		/usr/share/bdf2psf/ascii.set \
	)
	@bdf2psf --fb $(FONT) \
		 /usr/share/bdf2psf/standard.equivalents \
		 /usr/share/bdf2psf/ascii.set \
		 256 \
		 $(FONT_OUT)

.PHONY: defconfig
defconfig:
	@$(CURDIR)/scripts/default_config.py
	@echo "generated default config..."
