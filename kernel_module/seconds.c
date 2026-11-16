#include "linux/types.h"
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
MODULE_LICENSE("GPL");

static struct proc_dir_entry *proc_entry;
static unsigned long start_jiffies;
static ssize_t read_seconds(struct file *file, char __user *user_buffer,
                            size_t count, loff_t *offset) {

  unsigned long elapsed = (jiffies - start_jiffies) / HZ;
  char output[32];

  int len = scnprintf(output, sizeof(output), "%lu\n", elapsed);
  if (*offset > 0)
    return 0;
  if (copy_to_user(user_buffer, output, len))
    return -EFAULT;
  *offset = len;
  return len;
}
static const struct proc_ops pops = {
    .proc_read = read_seconds,
};
static int __init seconds_init(void) {
  printk(KERN_INFO "Module loaded");
  start_jiffies = jiffies;
  proc_entry = proc_create("seconds", 0666, NULL, &pops);

  return 0;
}

static void __exit seconds_exit(void) {
  proc_remove(proc_entry);
  printk(KERN_INFO "Module unloaded");
}

module_init(seconds_init);
module_exit(seconds_exit);
