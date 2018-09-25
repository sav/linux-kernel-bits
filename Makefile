include $(PWD)/config.mk

LKBVERSION := $(shell cat $(PWD)/VERSION)
LKBPATCHES := $(wildcard *.patch)
LKBMODULES := bh_tasklet bh_timer bh_workqueue mod_dummie

define ask
	test $(Y) || ( read -p "$1 " ans && test $$ans = 'y' ) && $2 || $3
endef

ifeq ($(V),)
	ARGSTAR=axf
	ARGSPATCH=-s
else
	ARGSTAR=axvf
	ARGSPATCH=
endif

all:
	@make source
	@make modules

modules:
	@for mod in $(LKBMODULES); do \
		make -C $(KBUILDDIR) M=$(PWD)/$${mod} modules; \
	done

install:
	@$(call ask,install modules to $(KBUILDDIR)?, \
		for mod in $(LKBMODULES); do \
			sudo make -C $(KBUILDDIR) M=$(PWD)/$${mod} \
				modules_install; \
		done, true)
	@make -C $(KSOURCEDIR) modules_install
	@make -C $(KSOURCEDIR) install
.PHONY: install

config:
	@$(call ask,really restore oldconfig?, \
		make -C $(KBUILDDIR)/ oldconfig && \
		make -C $(KBUILDDIR)/ prepare, true)
.PHONY: config

insmod:
	@for mod in $(LKBMODULES); do \
		$(call ask,load $${mod}.ko?, \
			sudo insmod $(PWD)/$${mod}/$${mod}.ko, true); \
	done
.PHONY: insmod

rmmod:
	@for mod in $(LKBMODULES); do \
		$(call ask,unload $${mod}?, sudo rmmod $${mod}, true); \
	done
.PHONY: rmmod

$(KSOURCEPKG):
	@curl -k -O $(KSOURCEURL)

$(KSOURCEDIR): $(KSOURCEPKG)
	@tar $(ARGSTAR) $(KSOURCEPKG)

$(KSOURCEDIR)/.config: $(KSOURCEDIR)
	@bash -c "$(KCATCONFIG) > $(KSOURCEDIR)/.config"

$(KSOURCEDIR)/vmlinux: $(KSOURCEDIR)/.config
	@make patch
	@make -C $(KSOURCEDIR) prepare
	@make -C $(KSOURCEDIR)
	@make -C $(KSOURCEDIR) modules

patch: $(KSOURCEDIR)
	@for patch in $(LKBPATCHES); do \
		$(call ask,apply $${patch}?, patch $(ARGSPATCH) \
			-d $(KSOURCEDIR) -p1 -l < $${patch}, true); \
	done

source: $(KSOURCEDIR)/vmlinux

clean:
	@for mod in $(LKBMODULES); do \
		make -C $(KBUILDDIR) M=$(PWD)/$${mod} clean; \
	done
	@make -C $(KBUILDDIR) M=$(PWD) clean
	@rm -rf modules.order $(KSOURCEPKG) $(KSOURCEDIR)
.PHONY: clean

help:
	@echo "  ===================`echo $(LKBVERSION) | tr [:print:] =`" 
	@echo "  Linux Kernel Bits $(LKBVERSION)"
	@echo "  ===================`echo $(LKBVERSION) | tr [:print:] =`" 
	@echo ""
	@echo " Config"
	@echo " ------"
	@echo "  version: $(KBUILDVERS)"
	@echo " builddir: $(KBUILDDIR)"
	@echo "   source: $(KSOURCEURL)"
	@echo "  modules: $(LKBMODULES)"
	@echo "  patches: $(LKBPATCHES)"
	@echo ""
	@echo " Usage"
	@echo " -----"
	@echo "     make [VAR=VAL,...] [RULE]"
	@echo ""
	@echo " Vars"
	@echo " ----"
	@echo "      V=1: verbose output (silent if omitted)"
	@echo "      Y=1: never ask for confirmation"
	@echo ""
	@echo " Rules"
	@echo " ----- "
	@echo "      all: build source and modules"
	@echo "   config: restore old config and prepare build"
	@echo "  modules: compile all modules"
	@echo "   insmod: load all modules"
	@echo "    rmmod: unload all modules"
	@echo "  install: install to build directory"
	@echo "   source: download source apply patches and build"
	@echo "    clean: clean up all build files"
	@echo "     help: show this help"
	@echo ""
.PHONY: help

