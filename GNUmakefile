.SUFFIXES:

override OUTPUT_BOOTLOADER = $(CURDIR)/build/boot/boot.efi
override OUTPUT_ISO = $(CURDIR)/os.iso
override OUTPUT_ESP = $(CURDIR)/build/boot/esp.img
override OUTPUT_KERNEL = $(CURDIR)/build/kernel

BOOT_TOOLCHAIN :=
BOOT_TOOLCHAIN_PREFIX := x86_64-w64-mingw32-

KERNEL_TOOLCHAIN :=
KERNEL_TOOLCHAIN_PREFIX :=

IS_EFI := $(shell [ -d /sys/firmware/efi ])

ARCH := $(shell uname -m)

-include config

.PHONY: all
all: $(OUTPUT_ISO)

$(OUTPUT_ISO): GNUmakefile
	$(MAKE) -C $(CURDIR)/arch/$(ARCH)/boot/uefi TOOLCHAIN=$(BOOT_TOOLCHAIN) TOOLCHAIN_PREFIX=$(BOOT_TOOLCHAIN_PREFIX)
	$(MAKE) -C $(CURDIR)/arch/$(ARCH)
	dd if=/dev/zero of=$(OUTPUT_ESP) bs=1k count=100000
	mkdosfs -F 32 $(OUTPUT_ESP)
	mmd -i $(OUTPUT_ESP) ::/EFI
	mmd -i $(OUTPUT_ESP) ::/EFI/BOOT
	mcopy -i $(OUTPUT_ESP) $(OUTPUT_BOOTLOADER) ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i $(OUTPUT_ESP) $(OUTPUT_KERNEL) ::/EFI/BOOT/kernel
	mkdir -p iso
	cp $(OUTPUT_ESP) iso
	xorriso \
		-as mkisofs \
		-R \
		-f \
		-e esp.img \
		-no-emul-boot \
		-o $(OUTPUT_ISO) \
		iso

.PHONY: clean
clean:
	rm -f $(OUTPUT_ESP)
	rm -f $(OUTPUT_BOOTLOADER)
	rm -f $(OUTPUT_ISO)
	rm -rf $(CURDIR)/build
	rm -rf $(CURDIR)/obj

.PHONY: submodules
submodules:
	$(MAKE) -C gnu-efi	
