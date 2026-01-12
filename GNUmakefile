.SUFFIXES:

override OUTPUT_BOOTLOADER = $(CURDIR)/build/boot/boot.efi
override OUTPUT_IMG = $(CURDIR)/os.iso
override OUTPUT_ESP = $(CURDIR)/build/boot/esp.img
override OUTPUT_KERNEL = $(CURDIR)/build/kernel

override OUTPUT_BOOT_SECT = $(CURDIR)/build/boot/boot_sect

BOOT_TOOLCHAIN :=
BOOT_TOOLCHAIN_PREFIX :=

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
		$(if $(filter $(2), 1),
			@echo $(2) # optional debug,
		)
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
all: $(OUTPUT_IMG)

.PHONY: uefi-img
uefi-img:
	@rm -f iso/*
	$(call log_if_notquiet, dd if=/dev/zero of=$(OUTPUT_ESP) bs=1k count=100000, "creating boot image...")
	$(call log_if_notquiet, mkdosfs -F 32 $(OUTPUT_ESP), "created FAT32 filesystem on boot image...")
	$(call log_if_notquiet, mmd -i $(OUTPUT_ESP) ::/EFI, "creating EFI root directory...")
	$(call log_if_notquiet, mmd -i $(OUTPUT_ESP) ::/EFI/BOOT, "creating EFI boot directory...")
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
			-o $(OUTPUT_IMG) \
			iso, \
		$(OUTPUT_IMG), \
		iso \
	)

.PHONY: bios-img
bios-img:
	@rm -f iso/*
	@mkdir -p iso
	$(call log_if_notquiet, cp $(OUTPUT_KERNEL) iso/)
	$(call log_if_notquiet, cp $(FONT_OUT) iso/spleen.psf)
	$(call log_if_notquiet, cp $(OUTPUT_BOOT_SECT) iso/boot_sect)
	$(call log_if_notquiet, truncate -s 1474560 iso/boot_sect)
	$(call log_step, \
		xorriso \
			-as mkisofs \
			-volid "VERYBASICOS" \
			-b boot_sect \
			-no-emul-boot \
			-boot-load-size 4 \
			-o $(OUTPUT_IMG) \
			iso, \
		$(OUTPUT_IMG), \
		iso \
	)

$(OUTPUT_IMG): GNUmakefile submodules
	$(call log_if_notquiet, \
		if [ ! -d $(CURDIR)/arch/$(ARCH)/boot/$(ARCH_BOOT_PLATFORM) ]; then \
			echo "boot platform $(ARCH_BOOT_PLATFORM) for arch $(ARCH) doesn't exist..."; \
		false; \
		fi, \
		"boot platform $(ARCH_BOOT_PLATFORM) for arch $(ARCH) doesn't exist...", \
	)
	@$(CURDIR)/scripts/check_tools.py --check-arch $(CURDIR)/arch $(ARCH)
	$(call add_flag_if_quiet, \
		$(MAKE) -C $(CURDIR)/arch/$(ARCH)/boot/$(ARCH_BOOT_PLATFORM) \
			-j$(CPUS) TOOLCHAIN_PREFIX=$(BOOT_TOOLCHAIN_PREFIX) \
			TOOLCHAIN=$(BOOT_TOOLCHAIN) LOG_QUIET=$(LOG_QUIET), \
		 --no-print-directory -s \
	)
	$(call add_flag_if_quiet, $(MAKE) -C $(CURDIR)/arch/$(ARCH) -j$(CPUS) LOG_QUIET=$(LOG_QUIET), --no-print-directory -s)
	$(call add_flag_if_quiet, \
		$(MAKE) \
			$(ARCH_BOOT_PLATFORM)-img \
			 LOG_QUIET=$(LOG_QUIET) # probably really bad code to make itself \
			CPUS=$(CPUS), \
		--no-print-directory -s \
	)
	@echo "build complete..."

.PHONY: clean
clean:
	@echo "cleaning all build artifacts..."
	@rm -f $(OUTPUT_ESP)
	@rm -f $(OUTPUT_BOOTLOADER)
	@rm -f $(OUTPUT_ISO)
	@rm -rf $(CURDIR)/build
	@rm -rf $(CURDIR)/obj
	@rm -rf $(CURDIR)/assets/fonts

.PHONY: distclean
distclean:
	@echo "cleaning all build artifacts including submodules..."
	@rm -f $(OUTPUT_ESP)
	@rm -f $(OUTPUT_BOOTLOADER)
	@rm -f $(OUTPUT_ISO)
	@rm -rf $(CURDIR)/build
	@rm -rf $(CURDIR)/obj
	@rm -rf $(CURDIR)/assets/fonts
	@git submodule foreach --recursive make clean || true

.PHONY: submodules
submodules:
	@$(if $(filter $(SKIP_TOOL_CHECK),0), $(CURDIR)/scripts/check_tools.py --exit,)
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
