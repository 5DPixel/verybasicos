# Configuration

The GNUmakefile includes a file titled `config`, specifying options including:
- `CFLAGS`
- `CPPFLAGS`
- `ARCH`
- `BOOT_TOOLCHAIN_PREFIX` (e.g. for uefi, this defaults to `x86_64-w64-mingw32-`
- `BOOT_TOOLCHAIN` (e.g. llvm)
- `IS_EFI`
- `LOG_QUIET` (suppress logging, useful if you need to debug)
- `SKIP_TOOL_CHECK` (skip check to see if tools exist in PATH)

You can generate a default config by running `make defconfig` from the root
