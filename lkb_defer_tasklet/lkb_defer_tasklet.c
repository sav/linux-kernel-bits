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
#include <linux/compiler.h>
#include <linux/interrupt.h>

#define COUNT_MAX 64

static void tasklet_handler(unsigned long);

DECLARE_TASKLET(tasklet, tasklet_handler, 0UL);
DECLARE_TASKLET(tasklet_hi, tasklet_handler, 1UL);

/* there's no need to use locks because both tasklets
 * run in the same per-cpu thread in ksoftirqd. tasklet_hi
 * will always be executed before tasklet. both handlers 
 * reschedule their tasklets to be executed right away
 * so the flow still stick to the same per-cpu thread.
 * also, since no memory is shared with any code in
 * interruption context there's no need to disable irqs.
 *
 * if we had smp but no irq handlers we would probally
 * use spink_lock(). theirs onerous counterpart,
 * spin_lock_irqsave(), are useful when dealing with
 * smp and irq handlers at the same time.
 *
 * finally, since again smp is not taking place in this
 * code we don't need to worry about out-of-order
 * execution, memory barriers and so on. let's start with
 * the simplest tasklet possible.
 *
 * despite the relaxed locking requirements tasklets still
 * can't sleep. if you need your defered work to sleep use
 * workqueues.
 */

static int count;

static void tasklet_handler(unsigned long is_hi)
{
	count++;
	if (count + 1 < COUNT_MAX) {
		if (is_hi) tasklet_hi_schedule(&tasklet_hi);
		else tasklet_schedule(&tasklet);
	}
}

static int __init lkb_defer_tasklet_init(void)
{
	pr_debug("loaded at 0x%p\n", lkb_defer_tasklet_init);
	pr_info("entering with count = %d\n", count);

	tasklet_schedule(&tasklet);
	tasklet_hi_schedule(&tasklet_hi);

	return 0;
}

static void __exit lkb_defer_tasklet_exit(void) 
{
	pr_info("exited with count = %d\n", count);
	pr_debug("unloaded from 0x%p\n", lkb_defer_tasklet_exit);
}

module_init(lkb_defer_tasklet_init);
module_exit(lkb_defer_tasklet_exit);

MODULE_LICENSE("GPL v2");
MODULE_VERSION("0.0.1");
MODULE_AUTHOR("sav <sav@loophole.cc>");
MODULE_DESCRIPTION(__FILE__);
