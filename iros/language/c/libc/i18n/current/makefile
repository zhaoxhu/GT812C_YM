# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /mnt/hgfs/shared/ht812c/ecos/ecos-2.1/packages
export PREFIX := /mnt/hgfs/shared/ht812c/iros/install
export COMMAND_PREFIX := arm-elf-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := language/c/libc/i18n/current
OBJECT_PREFIX := language_c_libc_i18n
CFLAGS := -mcpu=arm9 -mthumb -mthumb-interwork -O1 -Wall -Wstrict-prototypes -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fno-exceptions -fno-rtti -fno-exceptions -fvtable-gc
LDFLAGS := -mcpu=arm9 -mthumb -mthumb-interwork -Wl,-Map,map -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/ctype tests/setlocale tests/i18nmb

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/ctype.cxx src/locale.cxx src/mblen.cxx src/mbtowc.cxx src/mbstowcs.cxx src/wctomb.cxx src/wcstombs.cxx src/mbtowc_jp.cxx src/wctomb_jp.cxx src/mbtowc_c.cxx
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/libc/i18n/ctype.inl $(PREFIX)/include/cyg/libc/i18n/mb.h $(PREFIX)/include/cyg/libc/i18n/newlibctype.h

$(PREFIX)/include/cyg/libc/i18n/ctype.inl: $(REPOSITORY)/$(PACKAGE)/include/ctype.inl
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/i18n/mb.h: $(REPOSITORY)/$(PACKAGE)/include/mb.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/libc/i18n/newlibctype.h: $(REPOSITORY)/$(PACKAGE)/include/newlibctype.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

