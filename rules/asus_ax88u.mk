# (c) 2022 nearhop.com
# Makefile for compiling unum

TARGET_TOOLCHAIN := /opt/toolchains/crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1/

export STAGING_DIR=$(TARGET_TOOLCHAIN)

CC := arm-buildroot-linux-gnueabi-gcc
#CCLD := ld
CXX := arm-buildroot-linux-gnueabi-c++
GCC := arm-buildroot-linux-gnueabi-gcc
#LD := ld
STRIP := "$(TARGET_TOOLCHAIN)/bin/arm-buildroot-linux-gnueabi-strip"

TARGET_PLATFORM := arm
TARGET_PLATFORM_KARCH := arm
TARGET_PLATFORM_ARCH := arm-buildroot-linux-gnueabi


# Additional binary and libs search paths for the platform build toolchain
PATH_PREFIX=$(TARGET_TOOLCHAIN)/bin:$(TARGET_TOOLCHAIN)/$(TARGET_PLATFORM_ARCH)/bin:$(TARGET_TOOLCHAIN)/libexec/gcc/$(TARGET_PLATFORM_ARCH)/5.5.0

TARGET_CPPFLAGS := \
	-I$(TARGET_TOOLCHAIN)/lib/gcc/$(TARGET_PLATFORM_ARCH)/5.5.0/include \
	-I$(TARGET_TOOLCHAIN)/libexec/gcc/$(TARGET_PLATFORM_ARCH)/5.5.0/include \
	-I$(TARGET_TOOLCHAIN)/lib/gcc/$(TARGET_PLATFORM_ARCH)/5.5.0/include-fixed \
	-I$(TARGET_TOOLCHAIN)/lib/gcc/$(TARGET_PLATFORM_ARCH)/5.5.0/include \
	-I$(TARGET_TOOLCHAIN)/$(TARGET_PLATFORM_ARCH)/include/c++/5.5.0 \
	-I$(TARGET_TOOLCHAIN)/$(TARGET_PLATFORM_ARCH)/sys-include

TARGET_CFLAGS := \
	-pipe -funit-at-a-time -Wno-pointer-sign -marm -D_DEFAULT_SOURCE


####################################################################
# Lists of components to build and install for the platform        #
####################################################################

TARGET_LIST := unum # gdb
TARGET_INSTALL_LIST := $(TARGET_LIST) files

####################################################################


# Additional flags and vars for building the specific components

### unum
TARGET_CPPFLAGS_unum := \
	-I$(TARGET_LIBS)/libcurl/include \
	-I$(TARGET_LIBS)/bcmwifi/src/include \
	-I$(TARGET_LIBS)/bcmwifi/shared \
	-I$(TARGET_LIBS)/bcmwifi/bcmwifi/include \
	-I$(TARGET_LIBS)/bcmwifi/proto/include \
	-I$(TARGET_LIBS)/bcmwifi/wlioctl/include \
	-I$(TARGET_LIBS)/libjansson/include \
	-I$(TARGET_LIBS)/libshared/include \
	-I$(TARGET_LIBS)/libssl/include \

TARGET_LDFLAGS_unum := -ldl -lm -lrt \
	-L$(TARGET_LIBS)/libcurl/lib/ -l:libcurl.so \
	-L$(TARGET_LIBS)/libssl/lib/ -l:libssl.so \
	-L$(TARGET_LIBS)/libssl/lib/ -l:libcrypto.so \
        -L$(TARGET_LIBS)/bcmwifi/lib/ -l:libshared.so \
        -L$(TARGET_LIBS)/libjansson/lib/ -l:libjansson.so \
        -L$(TARGET_LIBS)/misc/lib/ -l:libnvram.so \
        -L$(TARGET_LIBS)/misc/lib/ -l:libwlcsm.so \
        
unum.install:
	mkdir -p "$(TARGET_RFS)/usr/bin"
	$(STRIP) -o "$(TARGET_RFS)/usr/bin/unum" "$(TARGET_OBJ)/unum/unum"

files.install:
	mkdir -p "$(TARGET_RFS)/usr/bin"
	# Install custom config and model files
	# Trusted CA list
	#cp -f "$(TARGET_FILES)/scripts/fw_upgrade.sh" "$(TARGET_RFS)/usr/bin/"
