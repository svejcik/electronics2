# This is a 'conventional' GNUmakefile until the make
# command is issued with the -C command below (see the default
# target).
#
ifeq ($(KERNELDIR),)
  KERNELDIR    := /lib/modules/$(shell uname -r)/build
endif

BINDIR = /scisw/daq/bin
PWD    := $(shell pwd)

#CC := gcc-4.1 
CC := gcc-4.4 
cflags-y = -O2

EXTRA_CFLAGS  := -O2 -DMODULE -D__KERNEL__ ${WARN} ${INCLUDE}
default:
ifneq ($(KERNELDIR),)
	@echo 'KERNELDIR DEFINED AS: '$(KERNELDIR)
else
	@echo 'KERNELDIR NOT DEFINED'
endif
	$(MAKE) CC=$(CC) -C $(KERNELDIR) M=$(PWD) modules

userspace: sport_mon_uspace_writetest sport_mon_uspace_readtest adc00_ctl

sport_mon_uspace_writetest: sport_debug_write_test.c
	gcc -lm -lncurses -o ${BINDIR}/sport_mon_uspace_writetest $?

adc00_ctl: sport01_adc00_write.c
	gcc -lm -lncurses -o ${BINDIR}/adc00_ctl $?

sport_mon_uspace_readtest: sport_debug_read_test.c
	gcc -lm -lncurses -o ${BINDIR}/sport_mon_uspace_readtest $?

sport_mon_uspace_readtest_debug: sport_debug_read_test2.c
	gcc -lm -lncurses -o ${BINDIR}/sport_mon_uspace_readtest_debug $?

sport_mon_uspace_readtest_v0.2: sport_debug_read_test_v0.2.c
	gcc -lm -lncurses -o ${BINDIR}/sport_mon_uspace_readtest_v0.2 $?

sport_mon_uspace_readtest_v0.3: sport_debug_read_test_v0.3.c
	gcc -lm -lncurses -o ${BINDIR}/sport_mon_uspace_readtest_v0.3 $?

sport_mon_uspace_readtest_v0.4: sport_debug_read_test4.c
	gcc -lm -lncurses -o ${BINDIR}/sport_mon_uspace_readtest_v0.4 $?

sport_track: sport_read_monitor.c
	gcc -lm -lncurses -o ${BINDIR}/sport_track $?

dummy_track: dummy_read.c
	gcc -lm -lncurses -o ${BINDIR}/serial_dummy_track $?

mycat: catserial.c
	gcc -lm -lncurses -o ${BINDIR}/catserial $?

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm *~

prep:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_prepare