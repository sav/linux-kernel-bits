obj-m += bh_timer.o

KBUILDVERS ?= $(shell uname -r)
KBUILDDIR ?= /lib/modules/$(KBUILDVERS)/build

EXTRA_CFLAGS += -ggdb3 -O1 -Werror -DDEBUG -I$(src)

all: module

module:
	make -C ${KBUILDDIR} M=$(PWD) modules

config:
	make -C ${KBUILDDIR} oldconfig && \
	make -C ${KBUILDDIR} prepare

install:
	sudo make -C ${KBUILDDIR} M=$(PWD) modules_install

clean:
	make -C ${KBUILDDIR} M=$(PWD) clean

.PHONY: config install clean
