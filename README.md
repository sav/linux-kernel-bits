Linux Kernel Bits v0.1
======================

Yet another collection of Linux Kernel snippets.

Config
------

Please edit `Makefile.inc` and set the variables accordingly 
then type `make help`.

Modules
-------

* bh_softirq: Defer to bottom half via Softirqs
* bh_tasklet: Defer to bottom half via Tasklets
* bh_workqueue: Defer to bottom half via Workqueues
* bh_timer: Defer to bottom half via Timers
* ex_module: Dummie example module

All modules print all debug messages prefixed by its name so it's easy to grep for their activity.

Planned
-------

* dbg_tracepoints: debug via trace/events/xxx.h
* rcu_list: simple RCU list usage example
