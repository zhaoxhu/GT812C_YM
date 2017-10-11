# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /mnt/hgfs/shared/ht812c/ecos/ecos-2.1/packages
export HOST := UNIX
export PREFIX := /mnt/hgfs/shared/ht812c/iros/install
export COMMAND_PREFIX := arm-elf-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export AR := $(COMMAND_PREFIX)ar

.PHONY: default build clean tests headers

build: headers $(PREFIX)/include/pkgconf/ecos.mak
	$(MAKE) -r -C hal/arm/arch/current arm.inc
	$(MAKE) -r -C services/memalloc/common/current heapgeninc.tcl
	$(MAKE) -r -C services/memalloc/common/current heaps.cxx
	$(MAKE) -r -C hal/arm/arch/current $@
	$(MAKE) -r -C hal/arm/immenstar/v2_0 $@
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C compat/posix/current $@
	$(MAKE) -r -C io/fileio/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C hal/arm/arm9/var/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C net/lwip_tcpip/current $@
	$(MAKE) -r -C io/eth/current $@
	$(MAKE) -r -C net/athttpd/current $@
	$(MAKE) -r -C services/crc/current $@
	$(MAKE) -r -C services/cpuload/current $@
	$(MAKE) -r -C fs/ram/current $@
	$(MAKE) -r -C fs/jffs2/current $@
	$(MAKE) -r -C io/flash/current $@
	$(MAKE) -r -C services/compress/zlib/current $@
	$(MAKE) -r -C compat/linux/current $@
	$(MAKE) -r -C devs/flash/intel/28fxxx/current $@
	$(MAKE) -r -C hal/common/current $(PREFIX)/lib/extras.o
	$(MAKE) -r -C hal/arm/arch/current $(PREFIX)/lib/vectors.o
	$(MAKE) -r -C hal/arm/arch/current $(PREFIX)/lib/target.ld
	@echo $@ finished

clean:
	$(MAKE) -r -C hal/arm/arch/current $@
	$(MAKE) -r -C hal/arm/immenstar/v2_0 $@
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C compat/posix/current $@
	$(MAKE) -r -C io/fileio/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C hal/arm/arm9/var/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C net/lwip_tcpip/current $@
	$(MAKE) -r -C io/eth/current $@
	$(MAKE) -r -C net/athttpd/current $@
	$(MAKE) -r -C services/crc/current $@
	$(MAKE) -r -C services/cpuload/current $@
	$(MAKE) -r -C fs/ram/current $@
	$(MAKE) -r -C fs/jffs2/current $@
	$(MAKE) -r -C io/flash/current $@
	$(MAKE) -r -C services/compress/zlib/current $@
	$(MAKE) -r -C compat/linux/current $@
	$(MAKE) -r -C devs/flash/intel/28fxxx/current $@
	@echo $@ finished

tests: build
	$(MAKE) -r -C hal/arm/arch/current $@
	$(MAKE) -r -C hal/arm/immenstar/v2_0 $@
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C compat/posix/current $@
	$(MAKE) -r -C io/fileio/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C hal/arm/arm9/var/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C net/lwip_tcpip/current $@
	$(MAKE) -r -C io/eth/current $@
	$(MAKE) -r -C net/athttpd/current $@
	$(MAKE) -r -C services/crc/current $@
	$(MAKE) -r -C services/cpuload/current $@
	$(MAKE) -r -C fs/ram/current $@
	$(MAKE) -r -C fs/jffs2/current $@
	$(MAKE) -r -C io/flash/current $@
	$(MAKE) -r -C services/compress/zlib/current $@
	$(MAKE) -r -C compat/linux/current $@
	$(MAKE) -r -C devs/flash/intel/28fxxx/current $@
	@echo $@ finished

headers:
	$(MAKE) -r -C hal/arm/arch/current $@
	$(MAKE) -r -C hal/arm/immenstar/v2_0 $@
	$(MAKE) -r -C hal/common/current $@
	$(MAKE) -r -C io/common/current $@
	$(MAKE) -r -C infra/current $@
	$(MAKE) -r -C kernel/current $@
	$(MAKE) -r -C services/memalloc/common/current $@
	$(MAKE) -r -C error/current $@
	$(MAKE) -r -C isoinfra/current $@
	$(MAKE) -r -C compat/posix/current $@
	$(MAKE) -r -C io/fileio/current $@
	$(MAKE) -r -C language/c/libc/common/current $@
	$(MAKE) -r -C language/c/libc/setjmp/current $@
	$(MAKE) -r -C language/c/libc/string/current $@
	$(MAKE) -r -C language/c/libc/startup/current $@
	$(MAKE) -r -C hal/arm/arm9/var/current $@
	$(MAKE) -r -C language/c/libc/i18n/current $@
	$(MAKE) -r -C language/c/libc/stdio/current $@
	$(MAKE) -r -C language/c/libc/stdlib/current $@
	$(MAKE) -r -C language/c/libc/time/current $@
	$(MAKE) -r -C io/wallclock/current $@
	$(MAKE) -r -C language/c/libm/current $@
	$(MAKE) -r -C net/lwip_tcpip/current $@
	$(MAKE) -r -C io/eth/current $@
	$(MAKE) -r -C net/athttpd/current $@
	$(MAKE) -r -C services/crc/current $@
	$(MAKE) -r -C services/cpuload/current $@
	$(MAKE) -r -C fs/ram/current $@
	$(MAKE) -r -C fs/jffs2/current $@
	$(MAKE) -r -C io/flash/current $@
	$(MAKE) -r -C services/compress/zlib/current $@
	$(MAKE) -r -C compat/linux/current $@
	$(MAKE) -r -C devs/flash/intel/28fxxx/current $@
	@echo $@ finished

$(PREFIX)/include/pkgconf/ecos.mak: makefile
	@echo 'ECOS_GLOBAL_CFLAGS = -mcpu=arm9 -mthumb -mthumb-interwork -O1 -Wall -Wstrict-prototypes -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fno-exceptions -fno-rtti -fno-exceptions -fvtable-gc' > $@
	@echo 'ECOS_GLOBAL_LDFLAGS = -mcpu=arm9 -mthumb -mthumb-interwork -Wl,-Map,map -g -nostdlib -Wl,--gc-sections -Wl,-static' >> $@
	@echo 'ECOS_COMMAND_PREFIX = $(COMMAND_PREFIX)' >> $@

