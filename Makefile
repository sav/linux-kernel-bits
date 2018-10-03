include $(PWD)/config.mk

LKBVERSION := $(shell cat $(PWD)/VERSION)
LKBPATCHES := $(wildcard *.patch)
LKBMODULES := lkb_defer_tasklet lkb_defer_timer \
	lkb_defer_workqueue lkb_dummie

define ask
	test $(Y) || ( read -p "$1 " ans && \
		test $$ans = 'y' ) &&  $2 || $3
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

um: $(KSOURCEDIR)
	@$(call ask,replace current build with usermode?, \
	    cp $(PWD)/.config.um $(KSOURCEDIR)/.config; \
	    make -C $(KSOURCEDIR) prepare ARCH=um; \
	    make -C $(KSOURCEDIR) ARCH=um, true)
.PHONY: um

install:
	@$(call ask,install modules to $(KBUILDDIR)?, \
		for mod in $(LKBMODULES); do \
			sudo make -C $(KBUILDDIR) M=$(PWD)/$${mod} \
				modules_install; \
		done, true)
		sudo cp -f $(KBUILDDIR)/System.map /boot
	@make install-source
.PHONY: install

install-source: $(KSOURCEDIR)/vmlinux
	@sudo make -C $(KSOURCEDIR) modules_install
	@sudo make -C $(KSOURCEDIR) install
.PHONY: install-source

config:
	@$(call ask,really restore oldconfig?, \
		make -C $(KBUILDDIR)/ oldconfig && \
		make -C $(KBUILDDIR)/ prepare, true)
.PHONY: config

load:
	@for mod in $(LKBMODULES); do \
		$(call ask,load $${mod}.ko?, \
			sudo insmod $(PWD)/$${mod}/$${mod}.ko, true); \
	done
.PHONY: load

unload:
	@for mod in $(LKBMODULES); do \
		$(call ask,unload $${mod}?, sudo rmmod $${mod}, true); \
	done
.PHONY: unload

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

clean-modules:
	@for mod in $(LKBMODULES); do \
		make -C $(KBUILDDIR) M=$(PWD)/$${mod} clean; \
	done
	@make -C $(KBUILDDIR) M=$(PWD) clean
	@rm -f modules.order
.PHONY: clean-modules

clean:
	make clean-modules
	@rm -rf $(KSOURCEPKG) $(KSOURCEDIR)
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
	@echo "      D=1: enable debug information"
	@echo ""
	@echo " Rules"
	@echo " ----- "
	@echo "      all: build everything"
	@echo "  modules: compile all modules"
	@echo "     load: load all modules"
	@echo "   unload: unload all modules"
	@echo "  install: install to prefix"
	@echo "       um: build user mode image"
	@echo "    clean: clean up all build files"
	@echo "     help: show this help"
	@echo ""
.PHONY: help

