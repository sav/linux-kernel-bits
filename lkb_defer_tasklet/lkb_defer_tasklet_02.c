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
#include <linux/atomic.h>
#include <linux/smp.h>

#define COUNT_MAX 64000000

static void tasklet_handler(unsigned long);

DECLARE_TASKLET(tasklet, tasklet_handler, 0UL);
DECLARE_TASKLET(tasklet_hi, tasklet_handler, 1UL);

static atomic_t count[1];
static int cpu = -1;

static void tasklet_handler(unsigned long is_hi)
{
	if (cpu != smp_processor_id()) pr_err("cpu changed!\n");

	atomic_inc(count);
	if (atomic_read(count) + 1 < COUNT_MAX) {
		if (is_hi) tasklet_hi_schedule(&tasklet_hi);
		else tasklet_schedule(&tasklet);
	}

	if (atomic_read(count) % 100000) {
		pr_info("cpu-%d: count = %d, %ssched()\n",
		        smp_processor_id(), atomic_read(count),
		        is_hi ? "hi_" : "");
	}
}

static int __init lkb_defer_tasklet_init(void)
{
	pr_debug("loaded at 0x%p\n", lkb_defer_tasklet_init);
	if (cpu == -1) cpu = smp_processor_id();

	tasklet_schedule(&tasklet);
	tasklet_hi_schedule(&tasklet_hi);

	return 0;
}

static void __exit lkb_defer_tasklet_exit(void) 
{
	pr_info("exited with count = %d\n", atomic_read(count));
	pr_debug("unloaded from 0x%p\n", lkb_defer_tasklet_exit);
}

module_init(lkb_defer_tasklet_init);
module_exit(lkb_defer_tasklet_exit);

MODULE_AUTHOR("sav <sav@loophole.cc>");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("0.0.1");
MODULE_DESCRIPTION(__FILE__);
