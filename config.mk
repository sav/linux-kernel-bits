# configuration file, feel free to make any changes.

CC           := clang
SHELL        := /bin/sh

ifneq (,$(D))
	EXTRA_CFLAGS := -O1 -Wall -Werr -DDEBUG -ggdb3
else
	EXTRA_CFLAGS := -O3 -Wall -Werr -DNDEBUG
endif

ifneq (4, $(shell uname -r | cut -f1 -d.))
	KBUILDVERS ?= 4.8
else
	KBUILDVERS ?= $(shell uname -r | grep -oE [0-9.]+ | head -1)
endif

KERNELVERSION = $(KBUILDDIR)
KBUILDDIR    ?= /lib/modules/$(KBUILDVERS)/build
KSOURCEVER   := $(KBUILDVERS)
KSOURCEPKG   := linux-$(KSOURCEVER).tar.xz
KSOURCEDIR   := $(KSOURCEPKG:.tar.xz=)
KSOURCEURL   := https://cdn.kernel.org/pub/linux/kernel/v4.x/$(KSOURCEPKG)

ifneq (,$(D))
	KCATCONFIG := cat $(PWD)/.config.dbg
else
	KCATCONFIG := cat $(PWD)/.config
endif
