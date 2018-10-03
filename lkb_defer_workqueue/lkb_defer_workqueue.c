/* Copyright (C) 2018, Savio Machado <sav@loophole.cc>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/**
 * if you need your bottom-half to sleep you need workqueues. workqueues
 * have all the benefits of the process context besides being schedulable.
 * it's usually used for example when your defered work needs to allocate
 * a lot of memory or perform blocking i/o. kernel threads would indeed be
 * an alternative to workqueues but the latter do have a much better api.
 * if you don't need a kernel thread or, if your defered work doesn't need
 * to sleep, use tasklets instead. see lkb_defer_tasklet.c.
 *
 * roughly speeking the workqueues subsystem actually provides an an
 * interface to create kernel threads tohandle work queued previously
 * from another part of the code. these kernel threads are called worker
 * threads. the default worker threads are called event/n where n is the
 * cpu number. there is one worker thread per cpu.
 */

static int __init lkb_defer_workqueue_init(void) {
	pr_debug("loaded at 0x%p\n", lkb_defer_workqueue_init);
	return 0;
}

static void __exit lkb_defer_workqueue_exit(void) {
	pr_debug("unloaded from 0x%p\n", lkb_defer_workqueue_exit);
}

module_init(lkb_defer_workqueue_init);
module_exit(lkb_defer_workqueue_exit);

MODULE_LICENSE("GPL v2");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION(__FILE__);
