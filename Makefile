
SUBDIRS := bh_softirq bh_tasklet bh_timer bh_workqueue ex_trivial

all:
	@for mod in $(SUBDIRS); do \
		make -C $(KBUILDDIR) M=$(PWD)/$${mod} modules; \
	done

install:
	@echo -n "install all: are you sure? "
	@read ANS && [ $$ANS == y ] && for mod in $(SUBDIRS); do \
		make -C $(KBUILDDIR) M=$(PWD)/$${mod} modules_install; \
	done

config:
	@echo -n "config kbuild: are you sure? "
	@read ANS && [ $$ANS == y ] && \
		( make -C $(KBUILDDIR)/ oldconfig && \
		  make -C $(KBUILDDIR)/ prepare )

insmod:
	@echo -n "insmod all: are you sure? "
	@read ANS && [ $$ANS == y ] && for mod in $(SUBDIRS); do \
		sudo insmod $(PWD)/$${mod}/$${mod}.ko; \
	done

rmmod:
	@echo -n "insmod all: are you sure? "
	@read ANS && [ $$ANS == y ] && for mod in $(SUBDIRS); do \
		sudo rmmod $${mod}; \
	done
clean:

	@for mod in $(SUBDIRS); do \
		make -C $(KBUILDDIR) M=$(PWD)/$${mod} clean; \
	done
	@make -C $(KBUILDDIR) M=$(PWD) clean
	@rm -f modules.order

# TODO pretty-list modules in help
help:
	@echo ""
	@echo "Linux Kernel Bits v0.1"
	@echo "======================"
	@echo ""
	@echo "Config"
	@echo "------"
	@echo "    KBUILDDIR=$(KBUILDDIR)"
	@echo "    KBUILDVERS=$(KBUILDVERS)"
	@echo ""
	@echo "Usage"
	@echo "-----"
	@echo "    make V=1 [RULE]"
	@echo ""
	@echo "Rules"
	@echo "-----"
	@echo "      all: build all modules (default rule)"
	@echo "  install: install to KBUILDDIR"
	@echo "   config: prepare KBUILDDIR config"
	@echo "   insmod: load all modules (requires root)"
	@echo "    clean: clean up build files"
	@echo "     help: show this help"
	@echo ""

.PHONY: all install config insmod clean help
