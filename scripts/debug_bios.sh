#!/bin/sh

qemu-system-x86_64 -M pc \
    -m 4G \
	-cdrom os.iso \
	-S \
	-gdb stdio
