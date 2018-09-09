CC := clang
SHELL := /bin/bash
EXTRA_CFLAGS += -ggdb3 -O1 -Werror -DDEBUG -I$(src)

KBUILDVERS ?= $(shell uname -r)
KBUILDDIR ?= /lib/modules/$(KBUILDVERS)/build
