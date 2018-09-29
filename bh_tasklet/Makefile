MODULE := bh_tasklet

CONFIGURE_MODULE_SIG=n

ifneq (,$(KERNELRELEASE))
	obj-m += $(MODULE).o
else
	include ../config.mk
endif

EXTRA_CFAGS += -I$(src)

all: $(MODULE).ko

$(MODULE).ko::
	make -C ${KBUILDDIR} M=$(PWD) modules

config:
	make -C ${KBUILDDIR} oldconfig && \
	make -C ${KBUILDDIR} prepare

install:
	sudo make -C ${KBUILDDIR} M=$(PWD) modules_install

load:
	@sudo insmod $(MODULE).ko

unload:
	@sudo rmmod $(MODULE) || true

clean:
	make -C ${KBUILDDIR} M=$(PWD) clean

.PHONY: load unload clean
