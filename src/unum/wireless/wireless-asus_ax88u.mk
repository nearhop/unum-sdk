# (c) 2022 nearhop.com
# Makefile for Asus AX88u wireless

# Add model specific include path to make wireless.h
# come from the right place.
CPPFLAGS += -I$(UNUM_PATH)/wireless/$(MODEL)

# Add code file(s)
OBJECTS += ./wireless/brcm_platform/radios_platform.o           \
           ./wireless/wireless_wl.o                             \
           ./wireless/brcm_platform/wireless_platform.o         \
           ./wireless/brcm_platform/stas_platform.o             \
           ./wireless/brcm_platform/scan_platform.o 

# Include common portion of the makefile for the subsystem
include ./wireless/wireless_common.mk

