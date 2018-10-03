
MODULES := lkb_defer_tasklet

CONFIGURE_MODULE_SIG=n

EXTRA_CFAGS += -I$(src)

ifneq (,$(KERNELRELEASE))
	obj-m += $(MODULES:=.o)
else
	include ../config.mk
endif

all: modules

modules:
	make -C ${KBUILDDIR} M=$(PWD) modules

config:
	make -C ${KBUILDDIR} oldconfig && \
	make -C ${KBUILDDIR} prepare

install:
	sudo make -C ${KBUILDDIR} M=$(PWD) modules_install

load:
	$(shell for mod in $(MODULES); do \
		sudo insmod $$mod.ko; \
	done)

unload:
	$(shell for mod in $(MODULES); do \
		sudo rmmod $$mod; \
	done)

clean:
	make -C ${KBUILDDIR} M=$(PWD) clean

.PHONY: load unload clean

