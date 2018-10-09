# configuration file, feel free to make any changes.

CC           := clang
SHELL        := /bin/bash
EXTRA_CFLAGS := -Wall -Werror

ifneq (,$(D))
	export EXTRA_CFLAGS += -O1 -ggdb3 -DDEBUG -DDEBUG_KERNEL -DDEBUG_INFO
else
	export EXTRA_CFLAGS += -O3
endif

ifneq (4, $(shell uname -r | cut -f1 -d.))
	KBUILDVERS ?= 4.8
else
	KBUILDVERS ?= $(shell uname -r | grep -oE [0-9.]+ | head -1)
endif

KBUILDDIR    ?= /lib/modules/$(KBUILDVERS)/build
KERNELVERSION = $(KBUILDDIR)
KSOURCEVER   := $(KBUILDVERS)
KSOURCEPKG   := linux-$(KSOURCEVER).tar.xz
KSOURCEDIR   := $(KSOURCEPKG:.tar.xz=)
KSOURCEURL   := https://cdn.kernel.org/pub/linux/kernel/v4.x/$(KSOURCEPKG)

ifneq (,$(D))
	KCATCONFIG := cat $(PWD)/.config.dbg
else
	KCATCONFIG := cat $(PWD)/.config
endif
