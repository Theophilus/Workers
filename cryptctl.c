#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

#include "crypt_ioctl.h"

MODULE_LICENSE("GPL");

static int times = 0;
static dev_t first;
static struct class *cl;
static struct cdev c_dev;

int dev_open(struct inode *, struct file *filep);
int dev_rls(struct inode *, struct file *filep);
static long crypt_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

static struct file_operations fops = {
  .open = dev_open,
  .release = dev_rls,
  .unlocked_ioctl = crypt_ioctl,
};


static long crypt_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  switch(cmd)
    {
    case CRYPT_READ:
      printk(KERN_INFO "called read\n");
      break;
      
    case CRYPT_WRITE:
      printk(KERN_INFO "called write\n");
      break;

    default:
      return -EINVAL;
    }
}

int init_module(void)
{
  printk(KERN_INFO "init_module() called\n");
  if (alloc_chrdev_region(&first, 0, 1, "cryptctl") < 0)
    {
      return -1;
    }
  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
      unregister_chrdev_region(first, 1);
      return -1;
    }
  if (device_create(cl, NULL, first, NULL, "cryptctl") == NULL)
    {
      class_destroy(cl);
      unregister_chrdev_region(first, 1);
      return -1;
    }
  cdev_init(&c_dev, &fops);
  if(cdev_add(&c_dev, first, 1) == -1)
    {
      device_destroy(cl, first);
      class_destroy(cl);
      unregister_chrdev_region(first, 1);
      return -1;
    }

  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "cleanup_module() called\n");
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first,1);
  
  printk(KERN_INFO "chr dev unregistered\n");
}

int dev_open(struct inode *inode, struct file *filep)
{
  times++;
  printk(KERN_INFO"Device opened %d times\n", times);
  return 0;
}

int dev_rls(struct inode *inode, struct file *filep)
{
  printk(KERN_INFO "Device closed\n");
  return 0;
}
