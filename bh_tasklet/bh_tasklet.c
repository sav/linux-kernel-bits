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

#define COUNT_MAX 64

static void tasklet_handler(unsigned long);

DECLARE_TASKLET(tasklet, tasklet_handler, 0UL);
DECLARE_TASKLET(tasklet_hi, tasklet_handler, 1UL);

/* there's no need to use locks because both tasklet and
 * tasklet_hi are guaranteed to run in the same thread and
 * no memory is shared among any interruption handler. 
 * if only the first case was false we could add spink_lock()
 * guards, which are less onerous than its counterpart
 * spin_lock_irqsave(). the latter would be required if both
 * of the cases were false. if only the second case is false
 * we would disable irq during all task_handler(). finally,
 * we use atomic_t to avoid out of order execution in smp.
 */
static atomic_t count[1];

static void tasklet_handler(unsigned long is_hi)
{
	atomic_inc(count);
	if (atomic_read(count) + 1 < COUNT_MAX) {
		if (is_hi) tasklet_hi_schedule(&tasklet_hi);
		else tasklet_schedule(&tasklet);
	}
}

static int __init bh_tasklet_init(void)
{
	pr_debug("loaded at 0x%p\n", bh_tasklet_init);
	pr_info("entering with count = %d\n", atomic_read(count));

	tasklet_schedule(&tasklet);
	tasklet_hi_schedule(&tasklet_hi);

	return 0;
}

static void __exit bh_tasklet_exit(void) 
{
	pr_info("exited with count = %d\n", atomic_read(count));
	pr_debug("unloaded from 0x%p\n", bh_tasklet_exit);
}

module_init(bh_tasklet_init);
module_exit(bh_tasklet_exit);

MODULE_AUTHOR("sav <sav@loophole.cc>");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("0.0.1");
MODULE_DESCRIPTION(__FILE__);
