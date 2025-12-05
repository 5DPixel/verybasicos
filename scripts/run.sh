#!/bin/sh

qemu-system-x86_64 -M pc \
	-m 4G \
	-drive if=pflash,format=raw,file=tools/ovmf/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=tools/ovmf/OVMF_VARS.fd \
	-drive if=none,id=usbstick,format=raw,file=os.iso \
	-usb \
	-device usb-ehci,id=ehci \
	-device usb-storage,bus=ehci.0,drive=usbstick
