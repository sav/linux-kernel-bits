Linux Kernel Bits v0.0.1
========================

Yet another collection of Linux Kernel snippets. This code was crafted to work with `linux-kernel-4.18.x` only. You can find the latest version in the link below.

* http://github.com/sav/linux-kernel-bits

Build
-----

Please edit `config.mk` and set the variables accordingly then either pick your choice from `make help` or go ahead with `make Y=1`. The latter might take a really long time.

Modules
-------

* `defer_tasklet`: Defer to bottom half via *Tasklets*
* `defer_workqueue`: Defer to bottom half via *Workqueues*
* `defer_timer`: Defer to bottom half via *Timers*
* `lkb_dummie`: Dummie example module

Patches
-------
* `defer_softirq.patch`: Defer to bottom half via *Softirq*. Softirqs are statically compiled thus a patch instead of a module

Planned
-------

* `dbg_tracepoints`: debug via *trace/events/xxx.h*
* `rcu_list`: *RCU List* API usage example
* `rcu_tree`: *RCU Tree* API usage example

To-Do
-----

* *v0.0.2* Build UML along with the bootable image
* *v0.0.2* Use local source to build the modules

