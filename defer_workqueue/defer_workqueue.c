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
#include <linux/workqueue.h>

/**
 * if you need your bottom-half to sleep you need workqueues. workqueues
 * have all the benefits of the process context besides being schedulable.
 * it's usually used for example when your defered work needs to allocate
 * a lot of memory or perform blocking i/o. kernel threads would indeed be
 * an alternative to workqueues but the latter do have a much better api.
 * if you don't need a kernel thread or, if your defered work doesn't need
 * to sleep, use tasklets instead. see defer_tasklet.c.
 *
 * roughly speeking the workqueues subsystem actually provides an an
 * interface to create kernel threads tohandle work queued previously
 * from another part of the code. these kernel threads are called worker
 * threads. the default worker threads are called event/n where n is the
 * cpu number. there is one worker thread per cpu.
 */

void somework(struct work_struct *);

DECLARE_WORK(somework_, somework);

void somework(struct work_struct *data)
{
	pr_info("somework()\n");
}

static int __init defer_workqueue_init(void)
{
	pr_debug("loaded at 0x%p\n", defer_workqueue_init);
	schedule_work(&somework_);
	return 0;
}

static void __exit defer_workqueue_exit(void)
{
	flush_scheduled_work();
	pr_debug("unloaded from 0x%p\n", defer_workqueue_exit);
}

module_init(defer_workqueue_init);
module_exit(defer_workqueue_exit);

MODULE_LICENSE("GPL v2");
MODULE_VERSION("0.0.1");
MODULE_AUTHOR("sav <sav@loophole.cc>");
MODULE_DESCRIPTION(__FILE__);
