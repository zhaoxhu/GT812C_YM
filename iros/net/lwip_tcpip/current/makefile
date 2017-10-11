# eCos makefile

# This is a generated file - do not edit

export REPOSITORY := /mnt/hgfs/shared/ht812c/ecos/ecos-2.1/packages
export PREFIX := /mnt/hgfs/shared/ht812c/iros/install
export COMMAND_PREFIX := arm-elf-
export CC := $(COMMAND_PREFIX)gcc
export OBJCOPY := $(COMMAND_PREFIX)objcopy
export HOST := UNIX
export AR := $(COMMAND_PREFIX)ar

PACKAGE := net/lwip_tcpip/current
OBJECT_PREFIX := net_lwip_tcpip
CFLAGS := -mcpu=arm9 -mthumb -mthumb-interwork -O1 -Wall -Wstrict-prototypes -Wpointer-arith -Winline -Wundef -g -ffunction-sections -fno-exceptions -fno-rtti -fno-exceptions -fvtable-gc
LDFLAGS := -mcpu=arm9 -mthumb -mthumb-interwork -Wl,-Map,map -g -nostdlib -Wl,--gc-sections -Wl,-static
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/src -I$(dir)/tests) -I.
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := tests/tcpecho tests/udpecho tests/httpd tests/socket tests/nc_test_slave tests/sys_timeout

build: headers libtarget.a.stamp

LIBRARY := libtarget.a
COMPILE := src/core/mem.c src/core/memp.c src/core/netif.c src/core/pbuf.c src/core/stats.c src/core/sys.c src/core/tcp.c src/core/tcp_in.c src/core/tcp_out.c src/core/inet.c src/core/ipv4/icmp.c src/core/ipv4/ip.c src/core/ipv4/ip_addr.c src/core/ipv4/ip_frag.c src/api/api_lib.c src/api/api_msg.c src/api/tcpip.c src/api/err.c src/api/sockets.c src/ecos/sys_arch.c src/ecos/init.c src/netif/loopif.c src/netif/etharp.c src/core/udp.c src/core/raw.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)))
	@cat $^ > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/lwipopts.h $(PREFIX)/include/network.h $(PREFIX)/include/arch/cc.h $(PREFIX)/include/arch/perf.h $(PREFIX)/include/arch/sys_arch.h $(PREFIX)/include/lwip/api.h $(PREFIX)/include/lwip/api_msg.h $(PREFIX)/include/lwip/arch.h $(PREFIX)/include/lwip/debug.h $(PREFIX)/include/lwip/def.h $(PREFIX)/include/lwip/dhcp.h $(PREFIX)/include/lwip/err.h $(PREFIX)/include/lwip/icmp.h $(PREFIX)/include/lwip/inet.h $(PREFIX)/include/lwip/ip.h $(PREFIX)/include/lwip/ip_addr.h $(PREFIX)/include/lwip/ip_frag.h $(PREFIX)/include/lwip/mem.h $(PREFIX)/include/lwip/memp.h $(PREFIX)/include/lwip/netif.h $(PREFIX)/include/lwip/opt.h $(PREFIX)/include/lwip/pbuf.h $(PREFIX)/include/lwip/raw.h $(PREFIX)/include/lwip/sio.h $(PREFIX)/include/lwip/snmp.h $(PREFIX)/include/lwip/sockets.h $(PREFIX)/include/lwip/stats.h $(PREFIX)/include/lwip/sys.h $(PREFIX)/include/lwip/tcp.h $(PREFIX)/include/lwip/tcpip.h $(PREFIX)/include/lwip/udp.h $(PREFIX)/include/netif/etharp.h $(PREFIX)/include/netif/loopif.h $(PREFIX)/include/netif/slipif.h

$(PREFIX)/include/lwipopts.h: $(REPOSITORY)/$(PACKAGE)/include/lwipopts.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/network.h: $(REPOSITORY)/$(PACKAGE)/include/network.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/arch/cc.h: $(REPOSITORY)/$(PACKAGE)/include/arch/cc.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/arch/perf.h: $(REPOSITORY)/$(PACKAGE)/include/arch/perf.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/arch/sys_arch.h: $(REPOSITORY)/$(PACKAGE)/include/arch/sys_arch.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/api.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/api.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/api_msg.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/api_msg.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/arch.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/arch.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/debug.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/debug.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/def.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/def.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/dhcp.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/dhcp.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/err.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/err.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/icmp.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/icmp.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/inet.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/inet.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/ip.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/ip.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/ip_addr.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/ip_addr.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/ip_frag.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/ip_frag.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/mem.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/mem.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/memp.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/memp.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/netif.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/netif.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/opt.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/opt.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/pbuf.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/pbuf.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/raw.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/raw.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/sio.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/sio.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/snmp.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/snmp.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/sockets.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/sockets.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/stats.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/stats.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/sys.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/sys.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/tcp.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/tcp.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/tcpip.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/tcpip.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/lwip/udp.h: $(REPOSITORY)/$(PACKAGE)/include/lwip/udp.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/netif/etharp.h: $(REPOSITORY)/$(PACKAGE)/include/netif/etharp.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/netif/loopif.h: $(REPOSITORY)/$(PACKAGE)/include/netif/loopif.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

$(PREFIX)/include/netif/slipif.h: $(REPOSITORY)/$(PACKAGE)/include/netif/slipif.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules.mak

