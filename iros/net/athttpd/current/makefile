# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /mnt/hgfs/shared/ht812c/ecos/ecos-2.1/packages
export PREFIX := /mnt/hgfs/shared/ht812c/iros/install
export COMMAND_PREFIX := arm-elf-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := net/athttpd/current
OBJECT_PREFIX := net_athttpd
CFLAGS := -mcpu=arm9 -mthumb -mthumb-interwork -O1 -Wall -Wstrict-prototypes -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fno-exceptions -fno-rtti -fno-exceptions -fvtable-gc -D__ECOS
LDFLAGS := -mcpu=arm9 -mthumb -mthumb-interwork -Wl,-Map,map -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := 

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/forms.c src/handler.c src/http.c src/socket.c src/cgi.c src/md5c.c src/auth.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/athttpd/auth.h $(PREFIX)/include/cyg/athttpd/digcalc.h $(PREFIX)/include/cyg/athttpd/forms.h $(PREFIX)/include/cyg/athttpd/global.h $(PREFIX)/include/cyg/athttpd/handler.h $(PREFIX)/include/cyg/athttpd/http.h $(PREFIX)/include/cyg/athttpd/md5.h $(PREFIX)/include/cyg/athttpd/socket.h $(PREFIX)/include/cyg/athttpd/cgi.h $(PREFIX)/include/cyg/athttpd/jim.h

$(PREFIX)/include/cyg/athttpd/auth.h: $(REPOSITORY)/$(PACKAGE)/include/auth.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/digcalc.h: $(REPOSITORY)/$(PACKAGE)/include/digcalc.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/forms.h: $(REPOSITORY)/$(PACKAGE)/include/forms.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/global.h: $(REPOSITORY)/$(PACKAGE)/include/global.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/handler.h: $(REPOSITORY)/$(PACKAGE)/include/handler.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/http.h: $(REPOSITORY)/$(PACKAGE)/include/http.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/md5.h: $(REPOSITORY)/$(PACKAGE)/include/md5.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/socket.h: $(REPOSITORY)/$(PACKAGE)/include/socket.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/cgi.h: $(REPOSITORY)/$(PACKAGE)/include/cgi.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/cyg/athttpd/jim.h: $(REPOSITORY)/$(PACKAGE)/include/jim.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

