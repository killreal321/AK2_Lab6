
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

static uint count = 1;

struct list_head *elem, *safe;

struct hello_data {
	struct list_head list;
	ktime_t time;
};

struct hello_data *data;

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
module_param(count, uint, 0660);
MODULE_PARM_DESC(count,  "The parameter shows how many times the message will be displayed.");

LIST_HEAD(list);

static int __init hello_init(void)
{

	if (count == 0) {
		pr_warn("Small value of 'count'!\n");
	} else if (count > 5 && count <= 10) {
		pr_warn("Big value of 'count'!\n");
	} else if (count > 0 && count <= 5) {
		int i = 0;

		for (; i < count; i++) {
			data = kmalloc(sizeof(*data), GFP_KERNEL);
			data->time = ktime_get();
			list_add_tail(&(data->list), &list);
			pr_emerg("Hello, world!\n");
		}
	} else {
		pr_err("Invalid value of 'count'!\n");
		return -EINVAL;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	list_for_each_safe(elem, safe, &list) {
		data = list_entry(elem, struct hello_data, list);
		pr_alert("ktime: %lu\n", (unsigned long) data->time);
		list_del(elem);
		kfree(data);
	}
}

module_init(hello_init);
module_exit(hello_exit);
