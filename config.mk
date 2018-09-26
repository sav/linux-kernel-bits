CC           := clang
SHELL        := /bin/sh
EXTRA_CFLAGS := -ggdb3 -O1 -Werror -DDEBUG

KBUILDVERS   ?= $(shell uname -r)
KBUILDDIR    ?= /lib/modules/$(KBUILDVERS)/build

KSOURCEVER   := 4.18
KSOURCEPKG   := linux-$(KSOURCEVER).tar.xz
KSOURCEDIR   := $(KSOURCEPKG:.tar.xz=)
KSOURCEURL   := https://cdn.kernel.org/pub/linux/kernel/v4.x/$(KSOURCEPKG)

ifneq (,$(wildcard /proc/config.gz))
KCATCONFIG   := zcat /proc/config.gz
else
KCATCONFIG   := cat $(PWD)/.config
endif
